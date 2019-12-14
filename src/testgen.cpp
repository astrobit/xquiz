#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <xquiz.h>
using namespace xquiz;

void Write_Form(const char * lpszFile,char chForm,const quiz & i_cQuiz, const quiz_instance & i_cInstance, bool i_bWrite_Notes, bool i_bWrite_QID )
{
	if (lpszFile != nullptr)
	{
		char sQuestion_Filename[256];
		char sSolutions_Filename[256];
		char sKey_Filename[256];
		sprintf(sQuestion_Filename,"%s_%c.tex",lpszFile,chForm);
		sprintf(sSolutions_Filename,"%s_%c_Solutions.tex",lpszFile,chForm);
		sprintf(sKey_Filename,"%s_%c_Key.tex",lpszFile,chForm);

		size_t tLine_Count_Form = 0;
		size_t tLine_Count_Solutions = 0;

		FILE * fileOut = fopen(sQuestion_Filename,"wt");
		FILE * fileKey = fopen(sKey_Filename,"wt");
		FILE * fileSolutions = fopen(sSolutions_Filename,"wt");
		if (fileOut != nullptr && fileKey != nullptr && fileSolutions != nullptr)
		{
			printf("Generating form %c\n",chForm);
			fprintf(fileOut,"\\documentclass{exam}\n\\usepackage{units}\n\\usepackage{tikz}\n\\usepackage{graphicx}\n\n\\newcommand{\\formcode}{Form %c}\n\n\\begin{document}\n\\title{\\LARGE",chForm);
			fprintf(fileOut,i_cQuiz.sTitle.c_str(),chForm);
			fprintf(fileOut,"}\n\\date{%s}\n\\maketitle\n%s\n\\newpage\n\\vspace*{\\fill}\n\\begin{center} This page intentionally left blank. \\end{center}\n\\vspace{\\fill}\n\\newpage\n\\begin{questions}\n",i_cQuiz.sDate.c_str(),i_cQuiz.sInstructions.c_str());

			fprintf(fileSolutions,"\\documentclass[answers]{exam}\n\\usepackage{units}\n\\usepackage{tikz}\n\\usepackage{graphicx}\n\n\\newcommand{\\formcode}{Form %c}\n\n\\begin{document}\n\\title{\\LARGE",chForm);
			fprintf(fileSolutions,i_cQuiz.sTitle.c_str(),chForm);
			fprintf(fileSolutions,"}\n\\date{%s}\n\\maketitle\n%s\n\\newpage\n\\vspace*{\\fill}\n\\begin{center} This page intentionally left blank. \\end{center}\n\\vspace{\\fill}\n\\newpage\n\\begin{questions}\n",i_cQuiz.sDate.c_str(),i_cQuiz.sInstructions.c_str());

			fprintf(fileKey,"\\documentclass{article}\n\\usepackage{fullpage}\n\\usepackage{pdflscape}\n\n\\newcommand{\\formcode}{Form %c}\n\n\\begin{document}\n",chForm);
			fprintf(fileKey, "\\begin{landscape}\\begin{centering}\n");
			fprintf(fileKey,i_cQuiz.sTitle.c_str(),chForm);
			fprintf(fileKey,"\\\\\n%s\\\\\n\\vspace{1cm}\n\\end{centering}\n\n\\begin{centering}\n",i_cQuiz.sDate.c_str());
			fprintf(fileKey, "\\begin{tabular}{|c||");

			size_t nNum_Questions = i_cInstance.vQuestions.size();
			size_t nNum_Columns_Key = nNum_Questions / 10;
			if ((nNum_Questions % 10) != 0)
				nNum_Columns_Key++;
			for(size_t nCol = 0; nCol < nNum_Columns_Key; nCol++)
			{
				fprintf(fileKey, "c|");
			}
			fprintf(fileKey, "}\n\\hline\n\t\t");
			for(size_t nCol = 0; nCol < nNum_Columns_Key; nCol++)
			{
				fprintf(fileKey, "&\t+%i\t",(nCol * 10));
			}
			fprintf(fileKey, "\\\\\n\\hline\n\\hline\n");

			std::map<size_t, char> mapAnswer_Key;

			size_t nQuestion = 1;


			for (auto iterI = i_cInstance.vQuestions.begin(); iterI != i_cInstance.vQuestions.end(); iterI++)
			{
				question cQuest = i_cQuiz.cBank.get(iterI->sID);

				// before starting to write the question, see if it will fit on the current page
				size_t tLine_Count_Prompt = cQuest.sPrompt.size() / 100 + 1;
				size_t tLine_Count_Answers = 0;
				size_t tLine_Count_Note = (cQuest.sNote.size() + 6) / 100 + 1;
				for (auto iterJ = iterI->sAnswer_IDs.begin();iterJ != iterI->sAnswer_IDs.end(); iterJ++)
				{
					answer cAns = cQuest.find_answer(*iterJ);
					tLine_Count_Answers += (cAns.sText.size() / 90) + 2;
				}
				size_t tNum_Lines_Question = tLine_Count_Prompt + tLine_Count_Answers + 1;
				if ((tNum_Lines_Question + tLine_Count_Form) > 110)
				{
					fprintf(fileOut,"\\newpage\n");
					tLine_Count_Form = tNum_Lines_Question;
				}
				else
					tLine_Count_Form += tNum_Lines_Question;
				if ((tNum_Lines_Question + tLine_Count_Solutions) > 110)
				{
					fprintf(fileSolutions,"\\newpage\n");
					tLine_Count_Solutions = tNum_Lines_Question + tLine_Count_Note;
				}
				else
					tLine_Count_Solutions += tNum_Lines_Question + tLine_Count_Note;
				
				
				fprintf(fileOut,"\\question");
				fprintf(fileSolutions,"\\question");
				fprintf(fileOut,"\\label{%s} ",iterI->sID.c_str());
				fprintf(fileSolutions,"\\label{%s} ",iterI->sID.c_str());
				if (i_bWrite_QID)
				{
					fprintf(fileOut,"(%s)",iterI->sID.c_str());
					fprintf(fileSolutions,"(%s)",iterI->sID.c_str());
				}
				fprintf(fileOut," %s\n\\begin{choices}\n",cQuest.sPrompt.c_str());
				fprintf(fileSolutions," %s\n\\begin{choices}\n",cQuest.sPrompt.c_str());
				char chLetter = 'a';
				for (auto iterJ = iterI->sAnswer_IDs.begin();iterJ != iterI->sAnswer_IDs.end(); iterJ++)
				{
					answer cAns = cQuest.find_answer(*iterJ);
					tLine_Count_Form++;
					tLine_Count_Solutions++;
					if (cAns.bCorrect)
					{
						fprintf(fileOut,"\\CorrectChoice ");
						fprintf(fileSolutions,"\\CorrectChoice ");
//						fprintf(fileKey,"\\item %c\n",chLetter);
						mapAnswer_Key[nQuestion] = chLetter + ('A' - 'a');
					}
					else
					{
						fprintf(fileOut,"\\choice ");
						fprintf(fileSolutions,"\\choice ");
					}
					fprintf(fileOut,"\\label{%s} ",cAns.sID.c_str());
					fprintf(fileSolutions,"\\label{%s} ",cAns.sID.c_str());
					fprintf(fileOut,"%s\n",cAns.sText.c_str());
					fprintf(fileSolutions,"%s\n",cAns.sText.c_str());

					chLetter++;
				}
				fprintf(fileOut,"\\end{choices}\n\n");
				fprintf(fileSolutions,"\\end{choices}\n");
				if (i_bWrite_Notes && !cQuest.sNote.empty())
					fprintf(fileSolutions,"\\textbf{Note:} %s\n",cQuest.sNote.c_str());
				fprintf(fileSolutions,"\n");
				nQuestion++;
			}

			for (size_t nRow = 0; nRow < 10; nRow++)
			{
				fprintf(fileKey,"\t%i\t",(nRow + 1));
				for (size_t nCol = 0; nCol <nNum_Columns_Key; nCol++)
				{
					nQuestion = nCol * 10 + nRow + 1;
					fprintf(fileKey,"&\t");
					if (nQuestion <= nNum_Questions) //<= because we start counting questions at 1
					{
						fprintf(fileKey,"%c",mapAnswer_Key[nQuestion]);
					}
					fprintf(fileKey,"\t");
				}
				fprintf(fileKey,"\\\\\n\\hline\n");
				if (nRow == 4)
					fprintf(fileKey,"\\hline\n");
			}
			fprintf(fileKey, "\\end{tabular}\n\\end{centering}\n\\end{landscape}\n");


			fprintf(fileOut,"\\end{questions}\n");
			fprintf(fileSolutions,"\\end{questions}\n");

			fprintf(fileKey,"\\end{document}\n");
			fprintf(fileOut,"\\end{document}\n");
			fprintf(fileSolutions,"\\end{document}\n");


			fclose(fileOut);
			fclose(fileSolutions);
			fclose(fileKey);
		}
	}
}

void Print_Options(FILE * file, const char * i_lpszCommand)
{
	fprintf(file,"Usage: %s [options] <filename>\n",i_lpszCommand);
	fprintf(file,"Create quizzes from quiz file.");
	fprintf(file,"Options:\n");
	fprintf(file,"\t-n X, --num X: generate X forms.\n");
	fprintf(file,"\t-f, --overwrite-makefile: overwrite makefile if necessary.\n");
	fprintf(file,"\t--dont-overwrite-makefile: don't overwrite makefile.\n");
	fprintf(file,"\t-nsq, --no-scramble-questions: don't scramble questions for each form.\n");
	fprintf(file,"\t-nsa, --no-scramble-answers: don't scramble answers for each question.\n");
	fprintf(file,"\t--shownotes: display notes (if available) in solution for each question.\n");
	fprintf(file,"\t-g, --no-gen, --dont-run-makefile: don't run the makefile.\n");
	fprintf(file,"\t--qid: show question ID for each question.\n");
}
int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	
	const char * lpszQuiz_File = nullptr;
	bool bScramble_Questions = true;
	bool bScramble_Answers = true;
	bool bInclude_Notes = false;
	bool bOverwrite_Makfile = false;
	bool bDont_Overwrite_Makefile = false;
	bool bDont_Run_Makefile = false;
	bool bShow_Question_ID = false;
	size_t tNum_Forms = 0;
	size_t tNum_Args = (size_t)i_iArg_Count;
	for (size_t tI = 1; tI < tNum_Args; tI++)
	{
		if (i_lpszArg_Values[tI][0] == '-')
		{
			if (strcmp(i_lpszArg_Values[tI],"--no-scramble-questions") == 0)
				bScramble_Questions = false;
			else if (strcmp(i_lpszArg_Values[tI],"-nsq") == 0)
				bScramble_Questions = false;
			else if (strcmp(i_lpszArg_Values[tI],"--no-scramble-answers") == 0)
				bScramble_Answers = false;
			else if (strcmp(i_lpszArg_Values[tI],"-nsa") == 0)
				bScramble_Answers = false;
			else if (strcmp(i_lpszArg_Values[tI],"--shownotes") == 0)
				bInclude_Notes = true;
			else if (strcmp(i_lpszArg_Values[tI],"--overwrite-makefile") == 0)
				bOverwrite_Makfile = true;
			else if (strcmp(i_lpszArg_Values[tI],"-f") == 0)
				bOverwrite_Makfile = true;
			else if (strcmp(i_lpszArg_Values[tI],"--dont-overwrite-makefile") == 0)
				bDont_Overwrite_Makefile = true;
			else if (strcmp(i_lpszArg_Values[tI],"--dont-run-makefile") == 0)
				bDont_Run_Makefile = true;
			else if (strcmp(i_lpszArg_Values[tI],"--no-gen") == 0)
				bDont_Run_Makefile = true;
			else if (strcmp(i_lpszArg_Values[tI],"-g") == 0)
				bDont_Run_Makefile = true;
			else if (strcmp(i_lpszArg_Values[tI],"--qid") == 0)
				bShow_Question_ID = true;
			else if (strncmp(i_lpszArg_Values[tI],"--num",5) == 0 || strncmp(i_lpszArg_Values[tI],"-n",2) == 0)
			{
				if (i_lpszArg_Values[tI][2] == '=')
				{
					if (i_lpszArg_Values[tI][3] >= '0' && i_lpszArg_Values[tI][3] <= '9')
						tNum_Forms = std::atoi(&i_lpszArg_Values[tI][3]);
					else if (i_lpszArg_Values[tI][3] == 0)
					{
						tI++;
						if (tI < tNum_Args && i_lpszArg_Values[tI][0] >= '0' && i_lpszArg_Values[tI][0] <= '9')
							tNum_Forms = std::atoi(i_lpszArg_Values[tI]);
						else
						{
							fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
							Print_Options(stderr,i_lpszArg_Values[0]);
						}
					}
					else
					{
						fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
						Print_Options(stderr,i_lpszArg_Values[0]);
					}
				}
				else if (i_lpszArg_Values[tI][2] == 0)
				{
					tI++;
					if (tI < tNum_Args)
					{
						if (i_lpszArg_Values[tI][0] >= '0' && i_lpszArg_Values[tI][0] <= '9')
							tNum_Forms = std::atoi(i_lpszArg_Values[tI]);
						else if (i_lpszArg_Values[tI][0] == '=')
						{
							if (i_lpszArg_Values[tI][1] >= '0' && i_lpszArg_Values[tI][1] <= '9')
								tNum_Forms = std::atoi(&i_lpszArg_Values[tI][1]);
							else if (i_lpszArg_Values[tI][1] == 0)
							{
								tI++;
								if (tI < tNum_Args)
								{
									if (i_lpszArg_Values[tI][0] >= '0' && i_lpszArg_Values[tI][0] <= '9')
										tNum_Forms = std::atoi(i_lpszArg_Values[tI]);
									else
									{
										fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
										Print_Options(stderr,i_lpszArg_Values[0]);
									}
								}
								else
								{
									fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
									Print_Options(stderr,i_lpszArg_Values[0]);
								}
							}
							else
							{
								fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
								Print_Options(stderr,i_lpszArg_Values[0]);
							}
						}
					}
					else
					{
						fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
						Print_Options(stderr,i_lpszArg_Values[0]);
					}
				}
				else
				{
					fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
					Print_Options(stderr,i_lpszArg_Values[0]);
				}
			}
			else if ((strcmp(i_lpszArg_Values[tI],"--help") == 0) || (strcmp(i_lpszArg_Values[tI],"-?") == 0))
			{
				Print_Options(stdout,i_lpszArg_Values[0]);
				return 1;
			}
			else
			{
				fprintf(stderr,"Unrecognized option %s\n",i_lpszArg_Values[tI]);
				Print_Options(stderr,i_lpszArg_Values[0]);
				return 1;
			}
		}
		else if ((strcmp(i_lpszArg_Values[tI],"help") == 0) || (strcmp(i_lpszArg_Values[tI],"Help") == 0) || (strcmp(i_lpszArg_Values[tI],"?") == 0))
		{
			Print_Options(stdout,i_lpszArg_Values[0]);
			return 1;
		}
		else 
			lpszQuiz_File = i_lpszArg_Values[tI];
	}
	if (lpszQuiz_File == nullptr)
	{
		fprintf(stderr,"No quiz file specified.\n");
		Print_Options(stderr,i_lpszArg_Values[0]);
		return 1;
	}


	if (lpszQuiz_File != nullptr)
	{
		char lpszFilename_Root[256];
		strcpy(lpszFilename_Root,lpszQuiz_File);
		char * lpszCursor = lpszFilename_Root;
		size_t tLen = 0;
		while (lpszCursor[0] != 0)
		{
			lpszCursor++;
			tLen++;
		}
		if (tLen > 4)
		{
			lpszCursor -= 4; // ".xml"
			lpszCursor[0] = 0; // cut off the .xml part
			tLen -= 4;
		}

		xmlDocPtr doc = xmlReadFile(lpszQuiz_File, nullptr, XML_PARSE_DTDVALID);

		if (doc == nullptr)
		{
			fprintf(stderr, "Failed to parse %s\n", i_lpszArg_Values[1]);
			return 1;
		}

		/*Get the root element node */
		quiz cQuiz(xmlDocGetRootElement(doc));
		if (cQuiz.cBank.size() == 0 || cQuiz.vsQuestion_IDs.size() == 0)
		{
			fprintf(stderr,"Problem loading quiz\n");
			return 1;
		}


		if (bScramble_Questions)
		{
			FILE * fileRand = fopen("/dev/urandom","rb");
			if (fileRand != nullptr)
			{
				unsigned int uiSeed;
				char * lpSeed = (char *)&uiSeed;
				for (size_t tI = 0; tI < sizeof(uiSeed); tI++)
				{
					lpSeed[tI] = fgetc(fileRand);
				}
				fclose(fileRand);
				srand(uiSeed);
			}
		}
		char lpszNum[512];
		size_t tNum = -1;
		if (tNum_Forms > 0)
			tNum = tNum_Forms;
		while (tNum == ((size_t)-1))
		{
			printf("How many test forms? ");
			char * lpRet = fgets(lpszNum,512,stdin);
			if (lpRet != lpszNum)
				fprintf(stderr,"Fault reading # of test forms.\n");
			lpszNum[511] = 0;
			if (lpszNum[0] >= '1' && lpszNum[0] <= '9')
				tNum = std::atoi(lpszNum);
		}
		char lpszResponse[512];


		std::map<size_t, quiz_instance> mapInstances;
		for (size_t tI = 0; tI < tNum; tI++)
		{
			char chForm = (char)('A' + tI);
			mapInstances[tI] = quiz_instance(cQuiz,bScramble_Questions,bScramble_Answers);

			Write_Form(lpszFilename_Root,chForm,cQuiz,mapInstances[tI],bInclude_Notes,bShow_Question_ID);
		}
		char lpszCorrelation_File_Path[512];
		sprintf(lpszCorrelation_File_Path,"%s_Question_Correlation.csv",lpszFilename_Root);
		FILE * fileCorrelation = fopen(lpszCorrelation_File_Path,"wt");
		if (fileCorrelation != nullptr)
		{
			fprintf(fileCorrelation,"Question ID");
			for (size_t tJ = 0; tJ < tNum; tJ++)
			{
				char chForm = (char)('A' + tJ);
				fprintf(fileCorrelation,", Form %c Question #",chForm);
				for (size_t tK = 0; tK < 5; tK++)
				{
					char chChoice = (char)('a' + tK);
					fprintf(fileCorrelation,", Form %c Question Choice %c",chForm,chChoice);
				}
			}
			fprintf(fileCorrelation,", Prompt");
			for (size_t tK = 0; tK < 5; tK++)
			{
				char chChoice = (char)('a' + tK);
				fprintf(fileCorrelation,", Choice %c ID, Choice %c text",chChoice,chChoice);
			}
			fprintf(fileCorrelation,"\n");

			for (size_t tI = 0; tI < mapInstances[0].vQuestions.size(); tI++)
			{
				fprintf(fileCorrelation,"%s",mapInstances[0].vQuestions[tI].sID.c_str());
				for (size_t tJ = 0; tJ < tNum; tJ++)
				{
					bool bFound = false;
					for (size_t tK = 0; tK < mapInstances[tJ].vQuestions.size() && !bFound; tK++)
					{
						if (mapInstances[tJ].vQuestions[tK].sID == mapInstances[0].vQuestions[tI].sID)
						{
							bFound = true;
							fprintf(fileCorrelation,", %zi",tK + 1);
							for (size_t tL = 0; tL < 5; tL++)
							{
								fprintf(fileCorrelation,", ");
								if (tL < mapInstances[tJ].vQuestions[tK].sAnswer_IDs.size())
								{
									fprintf(fileCorrelation,"\"%s\"",mapInstances[tJ].vQuestions[tK].sAnswer_IDs[tL].c_str());
								}
							}
						}
					}
				}
				question cQuest = cQuiz.cBank.get(mapInstances[0].vQuestions[tI].sID);
				fprintf(fileCorrelation,", \"%s\"",cQuest.sPrompt.c_str());
				for (size_t tL = 0; tL < 5; tL++)
				{
					fprintf(fileCorrelation,", ");
					if (tL < cQuest.num_answers())
					{
						answer cAnswer = cQuest.get_answer(tL);
						fprintf(fileCorrelation,"\"%s\"",cAnswer.sID.c_str());
					}
					fprintf(fileCorrelation,", ");
					if (tL < cQuest.num_answers())
					{
						answer cAnswer = cQuest.get_answer(tL);
						fprintf(fileCorrelation,"\"%s\"",cAnswer.sText.c_str());
					}
				}

				fprintf(fileCorrelation,"\n");
			}
			fclose(fileCorrelation);
		}

		char lpszMakefilePath[256];
		strcpy(lpszMakefilePath,lpszFilename_Root);
		tLen = 0;
		lpszCursor = lpszMakefilePath;
		while (lpszCursor[0] != 0)
		{
			lpszCursor++;
			tLen++;
		}

		while (tLen > 0 && lpszCursor[0] != '/' && lpszCursor[0] != '\\')
		{
			lpszCursor--;
			tLen--;
		}
		if (lpszCursor[0] == '/' || lpszCursor[0] == '\\')
			lpszCursor++;
		lpszCursor[0] = 0;
		strcpy(lpszCursor,"makefile");

		if (!bDont_Overwrite_Makefile)
		{

			FILE * fileMakefile = fopen(lpszMakefilePath,"rt");
			bool bOverwrite = true;
			if (fileMakefile != nullptr)
			{
				fclose(fileMakefile);
				if (!bOverwrite_Makfile)
				{
					printf("Overwrite makefile?\n");
					char * lpRet = fgets(lpszResponse,512,stdin);
					if (lpRet != lpszResponse)
						fprintf(stderr,"Fault reading response.\n");

					lpszResponse[511] = 0;
					// strip CR/LF

					lpszCursor = lpszResponse;
					tLen = 0;
					while (lpszCursor[0] != 0)
					{
						lpszCursor++;
						tLen++;
					}
					while (tLen > 0 && lpszCursor[0] < 26)
					{
						lpszCursor--;
						tLen--;
					}
					lpszCursor++;
					lpszCursor[0] = 0;
					bOverwrite = (strcmp(lpszResponse,"y") == 0 ||
						strcmp(lpszResponse,"Y") == 0 ||
						strcmp(lpszResponse,"yes") == 0 ||
						strcmp(lpszResponse,"Yes") == 0 ||
						strcmp(lpszResponse,"YEs") == 0 ||
						strcmp(lpszResponse,"YES") == 0 ||
						strcmp(lpszResponse,"t") == 0 ||
						strcmp(lpszResponse,"true") == 0 ||
						strcmp(lpszResponse,"True") == 0 ||
						strcmp(lpszResponse,"TRue") == 0 ||
						strcmp(lpszResponse,"TRUe") == 0 ||
						strcmp(lpszResponse,"TRUE") == 0);
				}
			}
			if (bOverwrite)
			{
				fileMakefile = fopen(lpszMakefilePath,"wt");
				if (fileMakefile != nullptr)
				{
					fprintf(fileMakefile,"all:");
					for (size_t tI = 0; tI < tNum; tI++)
					{
						char chForm = (char)('A' + tI);
						fprintf(fileMakefile," %s_%c.pdf",lpszFilename_Root,chForm);
						fprintf(fileMakefile," %s_%c_Key.pdf",lpszFilename_Root,chForm);
						fprintf(fileMakefile," %s_%c_Solutions.pdf",lpszFilename_Root,chForm);
					}
					fprintf(fileMakefile,"\n");

					for (size_t tI = 0; tI < tNum; tI++)
					{
						char chForm = (char)('A' + tI);
						fprintf(fileMakefile,"%s_%c.pdf: ",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"%s_%c.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile, "\n");
						fprintf(fileMakefile,"%s_%c_Key.pdf: ",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"%s_%c_Key.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Key.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Key.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Key.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile, "\n");
						fprintf(fileMakefile,"%s_%c_Solutions.pdf: ",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"%s_%c_Solutions.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Solutions.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Solutions.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile,"\txelatex %s_%c_Solutions.tex\n",lpszFilename_Root,chForm);
						fprintf(fileMakefile, "\n");
					}
					fprintf(fileMakefile,"clean:\n");
					fprintf(fileMakefile,"\t-rm *.pdf\n");
					fprintf(fileMakefile,"\t-rm *.aux\n");
					fprintf(fileMakefile,"\t-rm *.log\n");
					fclose(fileMakefile);
					printf("makefile generated. Type `make' to generate the .pdfs\n");
				}
			}
		}
		if (!bDont_Run_Makefile)
		{
			FILE * fileMakefile = fopen(lpszMakefilePath,"rt");
			if (fileMakefile != nullptr)
			{
				fclose(fileMakefile);
				char lpszMake_Command[512];
				sprintf(lpszMake_Command,"make -f %s",lpszMakefilePath);
				int nSuccess = system(lpszMake_Command);
				if (nSuccess != 0)
					fprintf(stderr,"Fault making pdf\n");
			}
		}
	}

	return 0;
}
