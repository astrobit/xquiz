#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>
#if defined (_WIN32) || defined (_WIN64)
#ifdef _USRDLL
#define PUBEXP _declspec(dllexport)
#else
#define PUBEXP _declspec(dllimport)
#endif
#else
#define PUBEXP
#endif


namespace xquiz
{

	class answer
	{
	public:
		std::string sID;
		bool bCorrect;
		std::string sText;
		bool bIs_Scramblable;

		PUBEXP virtual void Read_XML(xmlNode * i_lpRoot_Element);

		PUBEXP answer(void)
		{
			bCorrect = false;
			bIs_Scramblable = true;
		}

		PUBEXP answer(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = true;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_none : public answer
	{
	public:
		PUBEXP void Read_XML(xmlNode * i_lpRoot_Element);
		PUBEXP answer_none(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
		}
		PUBEXP answer_none(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_all : public answer
	{
	public:
		PUBEXP void Read_XML(xmlNode * i_lpRoot_Element);
		PUBEXP answer_all(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
		}
		PUBEXP answer_all(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			Read_XML(i_lpRoot_Element);
		}
	};
	class answer_some : public answer
	{
	public:
		std::string	szConjuction;

		PUBEXP void Read_XML(xmlNode * i_lpRoot_Element);
		PUBEXP answer_some(void)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			szConjuction = "and";
		}
		PUBEXP answer_some(xmlNode * i_lpRoot_Element)
		{
			bCorrect = false;
			bIs_Scramblable = false;
			szConjuction = "and";
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
		PUBEXP void Read_XML(xmlNode * i_lpRoot_Element);
		PUBEXP std::vector<std::string> get_keys(void) const{return vKeys;}
		PUBEXP std::vector<std::string>::const_iterator keys_begin(void) const {return vKeys.cbegin();}
		PUBEXP std::vector<std::string>::const_iterator keys_end(void) const {return vKeys.cend();}

	

		PUBEXP question & operator =(const question & i_cRHO)
		{
			Copy(i_cRHO);
			return *this;
		}
		PUBEXP question(xmlNode * i_lpRoot_Element)
		{
			vaAnswers.clear();
			mAnswers.clear();
			bScramble_Answers = false;
			Read_XML(i_lpRoot_Element);
		}
		PUBEXP question(void)
		{
			vaAnswers.clear();
			mAnswers.clear();
			bScramble_Answers = false;
		}
		PUBEXP question(const question & i_cRHO)
		{
			Copy(i_cRHO);
		}

		PUBEXP void add_answer(const answer & i_cAnswer)
		{
			mAnswers[i_cAnswer.sID] = vaAnswers.size();
			vaAnswers.push_back(i_cAnswer);
		}
		PUBEXP answer find_answer(std::string sID) const
		{
			answer cRet;
			if (mAnswers.count(sID) == 1)
			{
				size_t tIdx = mAnswers.at(sID);
				cRet = vaAnswers.at(tIdx);
			}
			return cRet;
		}
		PUBEXP size_t num_answers(void)
		{
			return vaAnswers.size();
		}
		PUBEXP answer get_answer(size_t i_tIdx) const
		{
			answer cRet;
			if (i_tIdx < vaAnswers.size())
				cRet = vaAnswers.at(i_tIdx);
			return cRet;
		}

		PUBEXP void Parse_XML(xmlNode * i_lpRoot_Element);

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
		PUBEXP qiter begin(void) const { return vQuestion_Bank.cbegin();}
		PUBEXP qiter end(void) const { return vQuestion_Bank.cend();}

	public:
		PUBEXP void Read_XML(xmlNode * i_lpRoot_Element);
		PUBEXP bank(xmlNode * i_lpRoot_Element)
		{
			Read_XML(i_lpRoot_Element);
		}

		PUBEXP bank(void)
		{
			vQuestion_Bank.clear();
		}
		PUBEXP bank(const bank & i_cRHO)
		{
			Copy(i_cRHO);
		}
		PUBEXP bank & operator	=(const bank & i_cRHO)
		{
			Copy(i_cRHO);
			return *this;
		}
		PUBEXP void add(const question & i_cQuestion)
		{
			vQuestion_Bank[i_cQuestion.sID] = i_cQuestion;
		}
		PUBEXP question get(const std::string &i_sID) const
		{
			question cRet;
			if (vQuestion_Bank.count(i_sID) == 1)
				cRet = vQuestion_Bank.at(i_sID);
			return cRet;
		}
		PUBEXP bank& operator +=(const bank & i_RHO)
		{
			for (auto iterI = i_RHO.vQuestion_Bank.begin(); iterI != i_RHO.vQuestion_Bank.end(); iterI++)
			{
				vQuestion_Bank[iterI->first] = iterI->second;
			}
			return *this;
		}
		PUBEXP bank operator +(const bank & i_cRHO) const
		{
			bank cRet(*this);
			cRet += i_cRHO;
			return cRet;
		}
		PUBEXP bool question_exists(const std::string & i_sID) const
		{
			bool bRet = (vQuestion_Bank.count(i_sID) == 1);
			return bRet;
		}
		PUBEXP size_t size(void) {return vQuestion_Bank.size();}

		PUBEXP void Parse_XML(xmlNode * i_lpRoot_Element);
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

		PUBEXP void Parse_XML(xmlNode * i_lpRoot_Element);
		PUBEXP void Validate_Questions(void);

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
		PUBEXP quiz_instance(void){;}
		PUBEXP quiz_instance(const quiz & i_cQuiz, bool i_bScramble_Questions, bool i_bScramble_Answers) {Generate_Instance(i_cQuiz,i_bScramble_Questions,i_bScramble_Answers);}
		PUBEXP void Generate_Instance(const quiz & i_cQuiz, bool i_bScramble_Questions, bool i_bScramble_Answers);
	};
};


