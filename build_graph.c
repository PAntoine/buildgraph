/*--------------------------------------------------------------------------------*
 * Name:  Build lookup graph
 *
 * This program will generate a look up graph for a set of words. 
 *
 * It will work out the search tree for the search of the input array the list
 * of functions.
 *
 * The algorithm for generating the tree is not optimised. The prune and tree
 * generation loops could be joined together.
 *
 * It also generates some code for searching the array. 
 *
 * PS: I know its a tree not a graph, but I thought that a graph was the 
 * answer, then realised it was a world of pain for little memory saving
 * so we now have a tree.
 *
 *-----------------------------------------------------------------------------{{{*
 * Author      Date       Version  Change
 * P.Antoine   14/03/2009 0.1      Initial version - There is a bug where I am
 *                                 Getting the odd empty row that is not
 *                                 referenced, an error in the pruning.
 * P.Antoine   29/04/2009 0.2      Laughing at myself, I need the table building
 *                                 stuff for another app, so functionalised the
 *                                 code so that I could use the routines for
 *                                 that. Also, noticed a couple of improvements
 *                                 that could be added.
 * P.Antoine   10/04/2009 0.3      Caved in, not happy that the compressed table
 *                                 is slower to use than the uncompressed table
 *                                 but it makes sense to allow for the uncompressed
 *                                 table to be exported.
 * P.Antoine   27/04/2010 0.4      Really cool bug on platforms that default to
 *                                 unsigned char constants. Need to force the 
 *                                 arrays and check value to signed or the table
 *                                 just does not work.
 * P.Antoine   04/09/2010 0.5      Minor tweaks to the code. Also, std make file.
 * P.Antoine   25/10/2010 0.6      Thought of a way of reducing the stupid amount
 *                                 of 0's in the table. But using a validation mask
 *                                 and then using the free space in the table as a
 *                                 lookup, if the tables don't clash. Will be able
 *                                 make the table considerably smaller. The small
 *                                 amount of testing shows at least 50% saving,
 *                                 have seen 75% (inc. the extra look-up code).
 * P.Antoine   06/11/2011 0.7      Fixed the way that the input filename is used.
 *                                 Also added "-q" so the thing is quiet and can
 *                                 be used in builds.
 * P.Antoine   06/11/2011 0.8      Added -h and made some error reporting better.
 * P.Antoine   10/11/2011 0.9      Added a function to report the string length.
 *-----------------------------------------------------------------------------}}}*
 * Peter Antoine - 9th March 2009.
 * Copyright 2009 (c) Peter Antoine.
 *--------------------------------------------------------------------------------*/

#if 0
#include <windows.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#endif

#include "build_graph.h"

/*-----------------------------------------------------------------------------*
 *  The string look up table.
 *-----------------------------------------------------------------------------*/
unsigned char look_uptable[ALPHABET_SIZE];

int	main(int argc,char *argv[])
{
	int	item = 1;
	int start = 1;
	int failed = 0;
	int result = 0;
	int infile;
	int quiet = 0;
	int num_symbols = 0;
	int ignore_case = 0;
	int enum_pre_size = 4;
	int have_filename = 0;
	int have_output_name = 0;
	int uncompressed_table = 0;
	unsigned int	num_of_words;
	unsigned int	data_read;
	unsigned int	file_size;
	unsigned int	total_data;
	unsigned int	*word_size;
	N_NODE			head_node;
	MASK_TABLE*		mask_table;

	char	*enum_prefix = "TST_";
	char	*header_dir = NULL;
	char	output_name[256];
	char	infile_name[256];
	char	**word;
	char	*data;
	char	*table;
	char	*error_string;
	char	*param_string;
	char	*compressed_table = NULL;

	memcpy(output_name,"output",sizeof("output"));

	error_string = "";
	param_string = "";

	output_name[0] = '\0';
	infile_name[0] = '\0';

	if (argc < 2)
	{
		failed = 1;
	}
	else
	{
		do
		{
			if (argv[start][0] == '-')
			{
				switch (argv[start][1])
				{
					case 'i':	/* ignore case independent */
							ignore_case = 1;
							break;
					
					case 'q':	/* quiet - suppress non error outputs */
							quiet = 1;
							break;


					case 'u':	/* produce the uncompressed table */
							uncompressed_table = 1;
							break;

					case 'h':	/* header directory */
							if (argv[start][2] != '\0')
							{
								header_dir = &argv[start][2];
							}
							else if (((start + 1) < argc) && argv[start+1][0] != '-')
							{
								start++;
								header_dir = argv[start];
							}
							else
							{
								error_string = "-h requires a directory name \n";
								failed = 1;
							}
							break;

					case 'p':	/* enum prefix */
							if (argv[start][2] != '\0')
							{
								enum_prefix = &argv[start][2];
								enum_pre_size = strlen(&argv[start][2]);
							}
							else if (((start + 1) < argc) && argv[start+1][0] != '-')
							{
								start++;
								enum_prefix = argv[start];
								enum_pre_size = strlen(argv[start]);
							}
							else
							{
								error_string = "p requires a prefix i.e. \"-pBGL_\" or \"-p BGL_\"\n";
								failed = 1;
							}
							break;

					case '?':	/* help  - just fail! */
							failed = 1;
							break;

					default:
							failed = 1;
							error_string = "Unknown parameter\n";
				}		

			}else{
				/* this should be positional parameters <input filename><output filename> */
				if (have_filename)
				{
					strncpy(output_name,argv[start],256);
					have_output_name = 1;
				}else{
					strncpy(infile_name,argv[start],256);
					have_filename = 1;
				}
			}

			start++;
		}
		while(start < argc);
	}

	if (have_filename && (infile = open(infile_name,O_RDONLY)) == -1)
	{
		error_string = "Failed to open file: %s \n";
		param_string = infile_name;
		failed = 1;
	}
	else
	{
		/* read in the list of words to calculate the table for */
		file_size = lseek(infile,0,SEEK_END);
		lseek(infile,0,SEEK_SET);

		if (file_size == 0)
		{
			error_string = "I think the input file is empty, so exiting.\n";
			failed = 1;
		}
	}

	if (!have_output_name)
	{
		error_string = "Must have output file name prefix\n";
		failed = 1;
	}
		
	if (!failed)
	{
		if (!quiet)
		{
			printf( "\n      build graph " __BG_VERSION__ "\n"
					"Copyright (c) 2009-2011 Peter Antoine\n"
					"        All rights reserved. \n\n");
			
			if (ignore_case)
			{
				printf("IGNORE CASE turned on. The table will be case independent.\n\n");
			}

			if (uncompressed_table)
			{
				printf("UNCOMPRESSED TABLES are turn on. The tables produced will be the uncompressed version.\n\n");
			}
		}

		/* allocate a multiple of 1k */
		data = (char*) malloc((((file_size+1023)/1024)*1024));
		total_data = 0;
		num_of_words = 0;

		/* now read the data */
		do
		{
			data_read = read(infile,data+total_data,1024);
			total_data += data_read;
		}
		while (total_data < file_size && data_read == 1024);

		/* now count the number of words in the file */
		num_of_words = count_words(data,total_data);
		
		word 		= (char**) malloc((sizeof(char*) * num_of_words));
		word_size	= (unsigned int*) malloc(sizeof(unsigned int)*num_of_words);

		if (word == NULL || word_size == NULL || num_of_words < 8)
		{
			error_string = "not enough words in the file, must be more than 8\n";
			failed = 1;
		}
		else
		{
			build_dictionary(num_of_words,data,total_data,word,word_size,ignore_case,quiet);
		}

		close(infile);
	}

	if (failed)
	{
		printf( "      build graph " __BG_VERSION__ "\n"
				" Copyright (c) 2009-2011 Peter Antoine\n"
				"        All rights reserved. \n\n");
		printf(error_string,param_string);
		printf("Usage:  %s <filename> [<prefix_for_output_filenames>] [-i] [-u] [-p <some_string>] [-h <directory>]\n",argv[0]);
		printf("           -i     The look-up will ignore the case of the word input.\n\n");
		printf("           -u     The uncompressed table will be exported.\n\n");
		printf("           -h     where to put the header file (if different from the source) .\n\n");
		printf("           -q     Only prints errors.\n\n");
		printf("           -p     Prefix for the enum in the include file.\n\n");

		result = 1;
	}
	else
	{
		memset((char*)&head_node,0,sizeof(N_NODE));

		build_naive_tree(&head_node,word,word_size,num_of_words);

		item = prune_tree(&head_node,word,word_size,num_of_words);

		table = (char*) malloc((item+1) * 255);
		memset(table,0,((item+1)*255));

		generate_naive_table(table,&head_node,word,word_size,num_of_words);

		if (!uncompressed_table)
		{
			num_symbols = compress_table(table,look_uptable,&compressed_table,item,word,word_size,num_of_words,ignore_case);

			mask_table = build_mask_table(compressed_table,item,num_symbols);
			build_output(output_name,header_dir,enum_prefix,compressed_table,look_uptable,item,num_symbols,word,word_size,num_of_words,ignore_case,mask_table,uncompressed_table);
		}
		else
		{
			if (ignore_case)
			{
				decase_table(table,item,ALPHABET_SIZE);
			}
			build_output(output_name,header_dir,enum_prefix,table,look_uptable,item,ALPHABET_SIZE,word,word_size,num_of_words,ignore_case,NULL,uncompressed_table);
		}

		/* release all the memory we have used */
		free_tree(&head_node);

		if (compressed_table != NULL)
		{
			free(compressed_table);
		}
	}

	return result;
}

/* vim: set fdm=marker: */
