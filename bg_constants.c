/***************************************************************************************
 *
 *       Filename:  bg_constants.c
 *
 *    Description:  This file holds the string constants for build the files.
 *
 *        Version:  1.0
 *        Created:  28/03/2009 10:40:08
 *       Revision:  none
 *
 *         Author:  Peter Antoine
 *          Email:  me@peterantoine.me.uk
 *
 *------------------------------------------------------------------------------------- 
 *                         Copyright (c) 2009 : Peter Antoine
 *                        Released under the Artistic License.
 ***************************************************************************************/

#include "build_graph.h"


/*-----------------------------------------------------------------------------*
 *  The string constants used to build the files.
 *-----------------------------------------------------------------------------*/
char	out_file_header[] = "/*--------------------------------------------------------------------------------*\n"
							" * Name: Parser Look-up tree\n"
							" * Desc: function and data structures to speed up a specific string search\n"
							" *\n"
							" *         **** DO NOT AMEND THIS CODE - IT IS AUTO_GENERATED ****\n"
							" *\n"
							" * Code and table produced by:\n"
							" *            build_graph \n"
							" *            version " __BG_VERSION__ "\n"
							" *\n"
							" *  written by Peter Antoine. \n"
							" *\n"
							" *   **** This code can be used however you like, no claims whatsoever are ****\n"
							" *   **** placed on the auto-generated code or data, or on the code that   ****\n"
							" *   **** uses the that code or data.                                      ****\n"
							" *   **** Or, to paraphrase \"Fill you're boots\".                           ****\n"
							" *--------------------------------------------------------------------------------*/\n\n"; 

char	out_file_start[]   ="\n\nint	";
char	out_file_function[]="_check_word(unsigned char* word)\n"
							"{\n"
							"	signed char	line = 0;\n"
							"	int	count = 0;\n"
							"	int letter = 0;\n"
							"\n"
							"	do\n"
							"	{\n"
							"		line = (signed char) state_machine_table[line][symbol_table[word[letter]]];\n"
							"		letter++;\n"
							"	}\n"
							"	while (line > 0 && word[letter] != '\\0');\n"
							"\n"
							"	if (line < 0)\n"
							"		line = (0-line) - 1;\n"
							"	else if ((line = (signed char) state_machine_table[line][symbol_table[0]]) < 0)\n"
							"		line = (0-line) - 1;\n"
							"	else\n"
							"		line = -1;\n"
							"\n";

char	out_file_uncompfunc[]="_check_word(char* word)\n"
							"{\n""	signed char	line = 0;\n"
							"	int	count = 0;\n"
							"	int letter = 0;\n"
							"\n"
							"	do\n"
							"	{\n"
							"		line = (signed char) state_machine_table[line][word[letter]];\n"
							"		letter++;\n"
							"\n"
							"	}\n"
							"	while (line >= 0 && word[letter] != '\\0');\n"
							"\n"
							"	if (line < 0)\n"
							"		line = (0-line) - 1;\n"
							"	else if ((line = (signed char) state_machine_table[line][0]) < 0)\n"
							"		line = (0-line) - 1;\n"
							"	else\n"
							"		line = -1;\n"
							"\n";

char	ignore_case_compare[] =	"	if (line >= 0)\n"
								"	{\n"
								"		for (count=letter;count < %s_table[line].length;count++)\n"
								"		{\n"
								"			if (toupper(word[count]) != ";

char	case_compare [] = 	"	if (line >= 0)\n"
							"	{\n"
							"		for (count=letter;count < %s_table[line].length;count++)\n"
							"		{\n"
							"			if (word[count] != ";

char	case_compare_end[]=	"_table[line].name[count])\n"
							"			{\n"
							"				line = -1;\n"
							"				break;\n"
							"			}\n"
							"		}\n"
							"	}\n"
							"	return line;\n"
							"}\n";

char	state_machine_table[] = "signed char\tstate_machine_table[";

/*-----------------------------------------------------------------------------*
 *  The size of the constants to stop C winging.
 *-----------------------------------------------------------------------------*/
unsigned int	out_file_header_size 		= sizeof(out_file_header);
unsigned int	out_file_start_size			= sizeof(out_file_start);
unsigned int	out_file_function_size		= sizeof(out_file_function);
unsigned int	ignore_case_compare_size	= sizeof(ignore_case_compare);
unsigned int	case_compare_size			= sizeof(case_compare);
unsigned int	case_compare_end_size		= sizeof(case_compare_end);
unsigned int	state_machine_table_size	= sizeof(state_machine_table);
unsigned int 	out_file_uncompfunc_size	= sizeof(out_file_uncompfunc);

