#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

namespace xquiz
{

	class answer
	{
	public:
		std::string sID;
		bool bCorrect;
		std::string sText;
		bool bIs_Scramblable;

		virtual void Read_XML(xmlNode * i_lpRoot_Element);

		answer(void)
		{
			bCorrect = false;
			bIs_Scramblable = true;
		}

		answer(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = true;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_none : public answer
	{
	public:
		void Read_XML(xmlNode * i_lpRoot_Element);
		answer_none(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
		}
		answer_none(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_all : public answer
	{
	public:
		void Read_XML(xmlNode * i_lpRoot_Element);
		answer_all(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
		}
		answer_all(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_some : public answer
	{
	public:
		void Read_XML(xmlNode * i_lpRoot_Element);
		answer_some(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
		}
		answer_some(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			Read_XML(i_lpRoot_Element);
		}
	};


	class question
	{
	private:
		std::vector<answer> vaAnswers;
		std::map<std::string, size_t> mAnswers;
		std::vector<std::string> vKeys;
	public:
		std::string sID;
		std::string sPrompt;
		bool bScramble_Answers;
		std::string sNote;
	private:
		void Copy(const question & i_cRHO)
		{
			sID = i_cRHO.sID;
			sPrompt = i_cRHO.sPrompt;
			bScramble_Answers = i_cRHO.bScramble_Answers;
			sNote = i_cRHO.sNote;
			vKeys = i_cRHO.vKeys;

			vaAnswers.clear();
			mAnswers.clear();
			for (size_t tI = 0; tI < i_cRHO.vaAnswers.size(); tI++)
			{
				mAnswers[i_cRHO.vaAnswers[tI].sID] = tI;
				vaAnswers.push_back(i_cRHO.vaAnswers[tI]);
			}
		}
		void parse_keys(std::string i_sKeys);
	public:
		void Read_XML(xmlNode * i_lpRoot_Element);
		std::vector<std::string> get_keys(void) const{return vKeys;}
		std::vector<std::string>::const_iterator keys_begin(void) const {return vKeys.cbegin();}
		std::vector<std::string>::const_iterator keys_end(void) const {return vKeys.cend();}

	

		question & operator =(const question & i_cRHO)
		{
			Copy(i_cRHO);
			return *this;
		}
		question(xmlNode * i_lpRoot_Element)
		{
			vaAnswers.clear();
			mAnswers.clear();
			bScramble_Answers = false;
			Read_XML(i_lpRoot_Element);
		}
		question(void)
		{
			vaAnswers.clear();
			mAnswers.clear();
			bScramble_Answers = false;
		}
		question(const question & i_cRHO)
		{
			Copy(i_cRHO);
		}

		void add_answer(const answer & i_cAnswer)
		{
			mAnswers[i_cAnswer.sID] = vaAnswers.size();
			vaAnswers.push_back(i_cAnswer);
		}
		answer find_answer(std::string sID) const
		{
			answer cRet;
			if (mAnswers.count(sID) == 1)
			{
				size_t tIdx = mAnswers.at(sID);
				cRet = vaAnswers.at(tIdx);
			}
			return cRet;
		}
		size_t num_answers(void)
		{
			return vaAnswers.size();
		}
		answer get_answer(size_t i_tIdx) const
		{
			answer cRet;
			if (i_tIdx < vaAnswers.size())
				cRet = vaAnswers.at(i_tIdx);
			return cRet;
		}

		void Parse_XML(xmlNode * i_lpRoot_Element);

	};


	class bank
	{
	private:
		std::map<std::string,question> vQuestion_Bank;

		void Copy(const bank & i_cRHO)
		{
			vQuestion_Bank.clear();
			for (auto iterI = i_cRHO.vQuestion_Bank.begin(); iterI != i_cRHO.vQuestion_Bank.end(); iterI++)
			{
				vQuestion_Bank[iterI->first] = iterI->second;
			}
		}
	public:
		typedef std::map<std::string,question>::const_iterator qiter;
		qiter begin(void) const { return vQuestion_Bank.cbegin();}
		qiter end(void) const { return vQuestion_Bank.cend();}

	public:
		void Read_XML(xmlNode * i_lpRoot_Element);
		bank(xmlNode * i_lpRoot_Element)
		{
			Read_XML(i_lpRoot_Element);
		}

		bank(void)
		{
			vQuestion_Bank.clear();
		}
		bank(const bank & i_cRHO)
		{
			Copy(i_cRHO);
		}
		bank & operator	=(const bank & i_cRHO)
		{
			Copy(i_cRHO);
		}
		void add(const question & i_cQuestion)
		{
			vQuestion_Bank[i_cQuestion.sID] = i_cQuestion;
		}
		question get(const std::string &i_sID) const
		{
			question cRet;
			if (vQuestion_Bank.count(i_sID) == 1)
				cRet = vQuestion_Bank.at(i_sID);
			return cRet;
		}
		bank& operator +=(const bank & i_RHO)
		{
			for (auto iterI = i_RHO.vQuestion_Bank.begin(); iterI != i_RHO.vQuestion_Bank.end(); iterI++)
			{
				vQuestion_Bank[iterI->first] = iterI->second;
			}
			return *this;
		}
		bank operator +(const bank & i_cRHO) const
		{
			bank cRet(*this);
			cRet += i_cRHO;
		}
		bool question_exists(const std::string & i_sID) const
		{
			bool bRet = (vQuestion_Bank.count(i_sID) == 1);
			return bRet;
		}
		size_t size(void) {return vQuestion_Bank.size();}

		void Parse_XML(xmlNode * i_lpRoot_Element);
	};

	class quiz
	{
	private:
		std::vector <std::string> Parse_Questions_XML(xmlNode * i_lpRoot_Element);

	public:
		std::string sTitle;
		std::string sDate;
		std::string sInstructions;
		bank		cBank;
		std::vector <std::string> vsQuestion_IDs;

		void Parse_XML(xmlNode * i_lpRoot_Element);
		void Validate_Questions(void);

		quiz(void)
		{
		}
		quiz(xmlNode * i_lpRoot_Element)
		{
			Parse_XML(i_lpRoot_Element);
		}
	};

	class question_instance
	{
	public:
		std::string sID;
		std::vector <std::string> sAnswer_IDs;
	};

	class quiz_instance
	{
	public:
		std::vector<question_instance> vQuestions;
		quiz_instance(void){;}
		quiz_instance(const quiz & i_cQuiz, bool i_bScramble_Questions, bool i_bScramble_Answers) {Generate_Instance(i_cQuiz,i_bScramble_Questions,i_bScramble_Answers);}
		void Generate_Instance(const quiz & i_cQuiz, bool i_bScramble_Questions, bool i_bScramble_Answers);
	};
};


