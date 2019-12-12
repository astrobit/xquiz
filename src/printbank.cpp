#include <cstdio>
#include <cstdlib>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <xquiz.h>

using namespace xquiz;

void Write_Form(const char * lpszFile,const bank & i_cBank )
{
	if (lpszFile != nullptr)
	{
		char sQuestion_Filename[256];
		sprintf(sQuestion_Filename,"%s.tex",lpszFile);

		FILE * fileOut = fopen(sQuestion_Filename,"wt");
		if (fileOut != nullptr)
		{
			fprintf(fileOut,"\\documentclass[answers]{exam}\n\\usepackage{units}\n\\usepackage{tikz}\n\\usepackage{graphicx}\n\n\\begin{document}\n\\title{\\LARGE Question Bank");
			fprintf(fileOut,"}\n\\date{\\today}\n\\maketitle\n\\begin{questions}\n");

			for (auto iterI = i_cBank.begin(); iterI != i_cBank.end(); iterI++)
			{
				question cQuest = iterI->second;

				fprintf(fileOut,"\\question (id: %s) (keys: ",cQuest.sID.c_str());
				bool bFirst = true;
				for (auto iterJ = cQuest.keys_begin(); iterJ != cQuest.keys_end(); iterJ++)
				{
					if (!bFirst)
						fprintf(fileOut,", ");
					fprintf(fileOut,"%s",iterJ->c_str());
					bFirst = false;
				}
				fprintf(fileOut,") %s\n\\begin{choices}\n",cQuest.sPrompt.c_str());
				char chLetter = 'a';
				for (size_t tJ = 0; tJ < cQuest.num_answers(); tJ++)
				{
					answer cAns = cQuest.get_answer(tJ);
					if (cAns.bCorrect)
					{
						fprintf(fileOut,"\\CorrectChoice ");
					}
					else
					{
						fprintf(fileOut,"\\choice ");
					}
					fprintf(fileOut,"(id:%s) %s\n",cAns.sID.c_str(),cAns.sText.c_str());

					chLetter++;
				}
				fprintf(fileOut,"\\end{choices}\n\n");
			}

			fprintf(fileOut,"\\end{questions}\n");

			fprintf(fileOut,"\\end{document}\n");


			fclose(fileOut);
		}
	}
}

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	
	if (i_iArg_Count == 2)
	{
		char lpszFilename_Root[256];
		strcpy(lpszFilename_Root,i_lpszArg_Values[1]);
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

		xmlDocPtr doc = xmlReadFile(i_lpszArg_Values[1], nullptr, XML_PARSE_DTDVALID);

		if (doc == nullptr)
		{
			fprintf(stderr, "Failed to parse %s\n", i_lpszArg_Values[1]);
			return 1;
		}

		/*Get the root element node */
		bank cBank(xmlDocGetRootElement(doc));
		std::map<std::string,std::string> mKeys;
		printf("------------------ Key list -----------------\n");
		for (auto iterI = cBank.begin(); iterI != cBank.end(); iterI++)
		{
			question cQuest = iterI->second;

			for (auto iterJ = cQuest.keys_begin(); iterJ != cQuest.keys_end(); iterJ++)
			{
				mKeys[*iterJ] = *iterJ;
			}
		}
		for (auto iterI = mKeys.begin(); iterI != mKeys.end(); iterI++)
			printf("%s\n",iterI->second.c_str());
			


		Write_Form(lpszFilename_Root,cBank);

	}

	return 0;
}
