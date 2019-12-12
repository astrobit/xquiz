#include <xquiz.h>
#include <sstream>
#include <iostream>
using namespace xquiz;

namespace xquiz_xml
{


	const char * Node_Get_PCDATA_Content(const xmlNode * i_lpNode)
	{
		const char * lpszRet = nullptr;
		xmlNode* lpText = i_lpNode->children;
		while (lpText != nullptr && lpText->type != XML_TEXT_NODE)
			lpText = lpText->next;
		if (lpText && lpText->content)
			lpszRet = (char *)lpText->content;
		return lpszRet;
	}
	inline bool Test_Attr_Content(const xmlAttr * i_lpAttr)
	{
		return (i_lpAttr != nullptr && i_lpAttr->children != nullptr && i_lpAttr->children->type == XML_TEXT_NODE);
	}

	std::string Attr_Get_String(const xmlAttr * i_lpAttr)
	{
		std::string sRet;
		if (Test_Attr_Content(i_lpAttr))
			sRet = (const char *)i_lpAttr->children->content;
		return sRet;
	}

	bool Attr_Get_Bool(const xmlAttr * i_lpAttr, bool i_bDefault)
	{
		bool bRet = i_bDefault;
		if (Test_Attr_Content(i_lpAttr))
		{
			bRet = (strcmp((char *)i_lpAttr->children->content,"true") == 0);
		}
		return bRet;
	}
	std::string Node_Get_Mixed_PCDATA_Content(const xmlNode * i_lpNode) // allows form, br, vspace, label, and ref elements to appear within
	{
		std::string sRet;
		xmlNode* lpText = i_lpNode->children;
		while (lpText != nullptr)
		{
			if (lpText->type == XML_TEXT_NODE && lpText->content != nullptr)
				sRet += (char*)(lpText->content);
			else if (lpText->type == XML_ELEMENT_NODE)
			{
				if (strcmp((char *)lpText->name,"form") == 0)
				{
					sRet += "\\formcode";
				}
				else if (strcmp((char *)lpText->name,"br") == 0)
					sRet += "\\\\";
				else if (strcmp((char *)lpText->name,"vspace") == 0)
				{
					xmlAttr * lpAttr = lpText->properties;
					while (lpAttr != nullptr)
					{
						if (strcmp((char *)lpAttr->name,"size") == 0)
						{
							sRet += "\\vspace{";
							sRet += Attr_Get_String(lpAttr);
							sRet += "}";
						}
						lpAttr =  lpAttr->next;
					}

				}
				else if (strcmp((char *)lpText->name,"label") == 0)
				{
					xmlAttr * lpAttr = lpText->properties;
					while (lpAttr != nullptr)
					{
						if (strcmp((char *)lpAttr->name,"id") == 0)
						{
							sRet += "\\label{";
							sRet += Attr_Get_String(lpAttr);
							sRet += "}";
						}
						lpAttr =  lpAttr->next;
					}
				}
				else if (strcmp((char *)lpText->name,"ref") == 0)
				{
					xmlAttr * lpAttr = lpText->properties;
					while (lpAttr != nullptr)
					{
						if (strcmp((char *)lpAttr->name,"id") == 0)
						{
							sRet += "\\ref{";
							sRet += Attr_Get_String(lpAttr);
							sRet += "}";
						}
						lpAttr =  lpAttr->next;
					}
				}
				else if (strcmp((char *)lpText->name,"fillin") == 0)
				{
					sRet += "\\fillin";
				}
				
			}
			lpText = lpText->next;
		}
		return sRet;
	}
};

void answer::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"choice") == 0)
	{
		sText = xquiz_xml::Node_Get_Mixed_PCDATA_Content(i_lpRoot_Element);
		xmlAttr * lpCurr_Attr = i_lpRoot_Element->properties;
		while (lpCurr_Attr)
		{
			if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
			{
				sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"scramble") == 0)
			{
				bIs_Scramblable = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,true);

			}
			else if (strcmp((char *)lpCurr_Attr->name,"correct") == 0)
			{
				bCorrect = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,false);
			}
			lpCurr_Attr = lpCurr_Attr->next;
		}
	}
}
void answer_none::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"choicenone") == 0)
	{
		xmlAttr * lpCurr_Attr = i_lpRoot_Element->properties;
		sText = "None of the above.";
		while (lpCurr_Attr)
		{
			if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
			{
				sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"correct") == 0)
			{
				bCorrect = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,false);
			}
			lpCurr_Attr = lpCurr_Attr->next;
		}
	}
}
void answer_all::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"choiceall") == 0)
	{
		xmlAttr * lpCurr_Attr = i_lpRoot_Element->properties;
		sText = "All of the above.";
		while (lpCurr_Attr)
		{
			if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
			{
				sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"correct") == 0)
			{
				bCorrect = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,false);
			}
			lpCurr_Attr = lpCurr_Attr->next;
		}
	}
}
void answer_some::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"choicesome") == 0)
	{
		xmlAttr * lpCurr_Attr = i_lpRoot_Element->properties;
		std::string sRefs;
		szConjuction = "and";
		while (lpCurr_Attr)
		{
			if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
			{
				sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"correct") == 0)
			{
				bCorrect = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,false);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"refs") == 0)
			{
				sRefs = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"conjunction") == 0)
			{
				std::string sConjuction_Lcl = xquiz_xml::Attr_Get_String(lpCurr_Attr);
				if (sConjuction_Lcl == "or")
					szConjuction = "or";
				else if (sConjuction_Lcl == "andor")
					szConjuction = "and/or";
			}

			lpCurr_Attr = lpCurr_Attr->next;
		}
		std::vector<std::string> vRefs;
		std::string sCurr_Ref;
		bool bWhitespace = true;
		for (auto iterI = sRefs.begin(); iterI != sRefs.end(); iterI++)
		{
			if (*iterI == ' ' || *iterI == ',' || *iterI == '\t')
			{
				bWhitespace = true;
				if (!sCurr_Ref.empty())
					vRefs.push_back(sCurr_Ref);
				sCurr_Ref.clear();
			}
			else if ((*iterI != ' ' && *iterI != '\t') || !bWhitespace)
			{
				sCurr_Ref.push_back(*iterI);
				bWhitespace = false;
			}
		}
		if (!sCurr_Ref.empty())
			vRefs.push_back(sCurr_Ref);
		std::ostringstream ossText;
		for (size_t tI = 0; tI < vRefs.size(); tI++)
		{
			if (tI > 0 && tI != (vRefs.size() - 1))
				ossText << ",";
			else if (tI == (vRefs.size() - 1))
				ossText << " " << szConjuction;
			ossText << " \\ref{" << vRefs[tI] << "}";
		}
		sText = ossText.str();

	}
}
void question::parse_keys(std::string i_sKeys)
{
	vKeys.clear();
	std::string sCurr_Key;
	bool bWhitespace = true;
	for (auto iterI = i_sKeys.begin(); iterI != i_sKeys.end(); iterI++)
	{
		if (*iterI == ',')
		{
			bWhitespace = true;
			if (!sCurr_Key.empty())
				vKeys.push_back(sCurr_Key);
			sCurr_Key.clear();
		}
		else if ((*iterI != ' ' && *iterI != '\t') || !bWhitespace)
		{
			sCurr_Key.push_back(*iterI);
			bWhitespace = false;
		}
	}
	if (!sCurr_Key.empty())
		vKeys.push_back(sCurr_Key);
}

void question::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"question") == 0)
	{
		xmlAttr * lpCurr_Attr = i_lpRoot_Element->properties;
		while (lpCurr_Attr)
		{
			if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
			{
				sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
			}
			else if (strcmp((char *)lpCurr_Attr->name,"keys") == 0)
			{
				parse_keys(xquiz_xml::Attr_Get_String(lpCurr_Attr));
			}
			lpCurr_Attr = lpCurr_Attr->next;
		}
		xmlNode * lpCurr_Node = i_lpRoot_Element->children;
		while (lpCurr_Node != nullptr)
		{

			switch (lpCurr_Node->type)
			{
			default:
				break;
			case XML_ELEMENT_NODE:
				if (strcmp((char *)lpCurr_Node->name,"prompt") == 0)
				{
					sPrompt = xquiz_xml::Node_Get_Mixed_PCDATA_Content(lpCurr_Node);
				}
				else if (strcmp((char *)lpCurr_Node->name,"note") == 0)
				{
					sNote = xquiz_xml::Node_Get_Mixed_PCDATA_Content(lpCurr_Node);
				}
				else if (strcmp((char *)lpCurr_Node->name,"choices") == 0)
				{
					xmlNode * lpCurr_Choice_Node = lpCurr_Node->children;
					xmlAttr * lpCurr_Attr = lpCurr_Node->properties;
					bool bCorrect_Answer = false;
					while (lpCurr_Attr)
					{
						if (strcmp((char *)lpCurr_Attr->name,"scramble") == 0)
						{
							bScramble_Answers = xquiz_xml::Attr_Get_Bool(lpCurr_Attr,true);
						}
						lpCurr_Attr = lpCurr_Attr->next;
					}
					while (lpCurr_Choice_Node != nullptr)
					{
						switch (lpCurr_Choice_Node->type)
						{
						default:
							break;
						case XML_ELEMENT_NODE:
							if (strcmp((char *)lpCurr_Choice_Node->name,"choice") == 0)
							{
								answer cAnswer(lpCurr_Choice_Node);
								bCorrect_Answer |= cAnswer.bCorrect;
								add_answer(cAnswer);
							}
							else if (strcmp((char *)lpCurr_Choice_Node->name,"choicenone") == 0)
							{
								answer_none cAnswer(lpCurr_Choice_Node);
								bCorrect_Answer |= cAnswer.bCorrect;
								add_answer(cAnswer);
							}
							else if (strcmp((char *)lpCurr_Choice_Node->name,"choiceall") == 0)
							{
								answer_all cAnswer(lpCurr_Choice_Node);
								bCorrect_Answer |= cAnswer.bCorrect;
								add_answer(cAnswer);
							}
							else if (strcmp((char *)lpCurr_Choice_Node->name,"choicesome") == 0)
							{
								answer_some cAnswer(lpCurr_Choice_Node);
								bCorrect_Answer |= cAnswer.bCorrect;
								add_answer(cAnswer);
							}
							break;
						} // switch choices child type
						lpCurr_Choice_Node = lpCurr_Choice_Node->next;
					} // while choices children
					if (!bCorrect_Answer)
						std::cerr << "Warning: no correct answer listed for " << sID << std::endl;
				} // if choices
				break;
			} // switch question child type
			lpCurr_Node = lpCurr_Node->next;
		} // while question children
	}
}
void bank::Read_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"bank") == 0)
	{
		xmlNode * lpCurr_Node = i_lpRoot_Element->children;

		while (lpCurr_Node != nullptr)
		{
			switch (lpCurr_Node->type)
			{
			default:
				break;
			case XML_ELEMENT_NODE:
				if (strcmp((char *)lpCurr_Node->name,"question") == 0)
				{
					add(question(lpCurr_Node));
				} // if question
				break;
			} // swtich bank child type
			lpCurr_Node = lpCurr_Node->next;
		} // while bank children
	} // if bank
}


std::vector <std::string> quiz::Parse_Questions_XML(xmlNode * i_lpRoot_Element)
{
	std::vector <std::string> cQ_List;
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"questions") == 0)
	{
		xmlNode * lpCurr_Node = i_lpRoot_Element->children;
		while (lpCurr_Node != nullptr)
		{
			switch (lpCurr_Node->type)
			{
			default:
				break;
			case XML_ELEMENT_NODE:
				if (strcmp((char *)lpCurr_Node->name,"qref") == 0)
				{
					xmlAttr * lpCurr_Attr = lpCurr_Node->properties;
					std::string sID;
					while (lpCurr_Attr)
					{
						if (strcmp((char *)lpCurr_Attr->name,"id") == 0)
						{
							sID = xquiz_xml::Attr_Get_String(lpCurr_Attr);
						}
						lpCurr_Attr = lpCurr_Attr->next;
					}
					cQ_List.push_back(sID);
				}
				break;
			}
			lpCurr_Node = lpCurr_Node->next;
		} // while
	} // if node  
	return cQ_List;
}
void quiz::Validate_Questions(void)
{
	for (auto iterI = vsQuestion_IDs.begin(); iterI != vsQuestion_IDs.end(); iterI++)
	{
		if (cBank.question_exists(*iterI) != 1)
			fprintf(stderr,"Unable to find question with ID %s in bank.\n",iterI->c_str());
	}
}

void quiz::Parse_XML(xmlNode * i_lpRoot_Element)
{
	if (i_lpRoot_Element && i_lpRoot_Element->type == XML_ELEMENT_NODE && strcmp((char *)i_lpRoot_Element->name,"quiz") == 0)
	{
		xmlNode * lpCurr_Node = i_lpRoot_Element->children;
		// first go through and identify all sourcefiles, colors, and axes
		while (lpCurr_Node != nullptr)
		{
			switch (lpCurr_Node->type)
			{
			default:
				break;
			case XML_ELEMENT_NODE:
				if (strcmp((char *)lpCurr_Node->name,"title") == 0)
				{
					sTitle = xquiz_xml::Node_Get_Mixed_PCDATA_Content(lpCurr_Node);
				}
				else if (strcmp((char *)lpCurr_Node->name,"date") == 0)
				{
					sDate = xquiz_xml::Node_Get_PCDATA_Content(lpCurr_Node);
				}
				else if (strcmp((char *)lpCurr_Node->name,"datealt") == 0)
				{
					xmlAttr * lpCurr_Attr = lpCurr_Node->properties;
					std::string sDay;
					std::string sMonth;
					std::string sYear;
					while (lpCurr_Attr)
					{
						if (strcmp((char *)lpCurr_Attr->name,"day") == 0)
						{
							sDay = xquiz_xml::Attr_Get_String(lpCurr_Attr);
						}
						else if (strcmp((char *)lpCurr_Attr->name,"month") == 0)
						{
							sMonth = xquiz_xml::Attr_Get_String(lpCurr_Attr);
						}
						else if (strcmp((char *)lpCurr_Attr->name,"year") == 0)
						{
							sYear = xquiz_xml::Attr_Get_String(lpCurr_Attr);
						}
						lpCurr_Attr = lpCurr_Attr->next;
					}
					sDate = sDay + "~" + sMonth + "~" + sYear;
				}
				else if (strcmp((char *)lpCurr_Node->name,"instructions") == 0)
				{
					sInstructions = xquiz_xml::Node_Get_Mixed_PCDATA_Content(lpCurr_Node);
				}
				else if (strcmp((char *)lpCurr_Node->name,"bank") == 0)
				{
					cBank += bank(lpCurr_Node);
				} // if bank
				else if (strcmp((char *)lpCurr_Node->name,"questions") == 0)
				{
					vsQuestion_IDs = Parse_Questions_XML(lpCurr_Node);
				} // if bank
				else if (strcmp((char *)lpCurr_Node->name,"bankref") == 0)
				{
					xmlAttr * lpCurr_Attr = lpCurr_Node->properties;
					std::string sBank_File;
					while (lpCurr_Attr)
					{
						if (strcmp((char *)lpCurr_Attr->name,"file") == 0)
						{
							sBank_File = xquiz_xml::Attr_Get_String(lpCurr_Attr);
						}
						lpCurr_Attr = lpCurr_Attr->next;
					}
					xmlDocPtr bank_doc = xmlReadFile(sBank_File.c_str(), nullptr, XML_PARSE_DTDVALID);

					if (bank_doc == nullptr)
					{
						fprintf(stderr, "Failed to parse %s\n", sBank_File.c_str());
						return;
					}
					cBank += bank(xmlDocGetRootElement(bank_doc));
				} // if bank
				break;
			} // switch root child type
			lpCurr_Node = lpCurr_Node->next;
		} // while root children
	} // if ..
	Validate_Questions();
}


void quiz_instance::Generate_Instance(const quiz & i_cQuiz, bool i_bScramble_Questions, bool i_bScramble_Answers)
{
	std::map<size_t,question_instance> mapQuestions;
	size_t tOrder = 1;

	for (size_t tI = 0; tI < i_cQuiz.vsQuestion_IDs.size(); tI++)
	{
		question cQuest = i_cQuiz.cBank.get(i_cQuiz.vsQuestion_IDs[tI]);
		question_instance qCurr;
		qCurr.sID = cQuest.sID;
		if (i_bScramble_Answers && cQuest.bScramble_Answers)
		{
			std::map<size_t,std::string> mapAnswers;
			for (size_t tJ = 0; tJ < cQuest.num_answers(); tJ++)
			{
				if (cQuest.get_answer(tJ).bIs_Scramblable)
					mapAnswers[rand()] = cQuest.get_answer(tJ).sID;

			}
			auto iterK = mapAnswers.begin();
			for (size_t tJ = 0; tJ < cQuest.num_answers(); tJ++)
			{
				if (cQuest.get_answer(tJ).bIs_Scramblable)
				{
					qCurr.sAnswer_IDs.push_back(iterK->second);
					iterK++;
				}
				else
				{
					qCurr.sAnswer_IDs.push_back(cQuest.get_answer(tJ).sID);
				}
			}

		}
		else
		{
			for (size_t tJ = 0; tJ < cQuest.num_answers(); tJ++)
			{
				qCurr.sAnswer_IDs.push_back(cQuest.get_answer(tJ).sID);
			}
		}
		size_t tCurr_Num = tOrder;
		if (i_bScramble_Questions)
			tCurr_Num = rand();
		mapQuestions[tCurr_Num] = qCurr;
		tOrder++;
	}
	for (auto iterI = mapQuestions.begin(); iterI != mapQuestions.end(); iterI++)
	{
		vQuestions.push_back(iterI->second);
	}
}

