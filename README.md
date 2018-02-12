# xquiz
XML quiz format, reader, and quiz generator.
This system uses an XML question bank format (specified by xquiz-bank.dtd) and XML quiz format (xquiz.dtd) to generate quizzes. xquiz-gen generates multiple quiz forms, all with the same set of questions but with the question order and answers scrambled.

git repository: https://github.com/astrobit/xquiz

Prerequisites:
	libxml2-devel
	some form of LaTeX that includes pdflatex
		Note: this is a softer requirement -- xquiz-gen and xquiz-bank will create makefiles that assume
		you have pdflatex installed. You may manually modify these to use whichever LaTeX compiler that you prefer.
	The exam class for LaTeX (see your LaTeX package manager; I am using texlive-exam)

Installation Instructions:

./configure
make
make install

Binaries:
	xquiz-gen: generates quiz forms from a xquiz type xml file and optionally runs latex to generate the .pdf forms. A correlation file (.csv) is also created in order to know which questions and choices occur in which order on each form. Additionally, a solutions file is generated that can contain optional notes (explanations) for each question. Finally, a key file is generated for each form that lists only the correct letter answer of each question.
	xquiz-bank: generates a latex exam containing all questions in the specified question bank, with answers highlighted


