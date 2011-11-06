/***************************************************************************************
 *
 *       Filename:  bg_output.c
 *
 *    Description:  This file holds the output function for the build function.
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
#ifdef win32
#include <io.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include <libgen.h>
#include "build_graph.h"

/*-----------------------------------------------------------------------------*
 *  The external strings for the files
 *-----------------------------------------------------------------------------*/
extern char	out_file_start[];
extern char	out_file_function[];
extern char	ignore_case_compare[];
extern char	case_compare[];
extern char	case_compare_end[];
extern char	state_machine_table[];
extern char	hex_char[];
extern char	out_file_header[];
extern char	out_file_uncompfunc[];

/*-----------------------------------------------------------------------------*
 *  The size of the constants to stop C winging.
 *-----------------------------------------------------------------------------*/
extern unsigned int	out_file_header_size;
extern unsigned int	out_file_start_size;
extern unsigned int	out_file_function_size;
extern unsigned int	ignore_case_compare_size;
extern unsigned int	case_compare_size;
extern unsigned int	case_compare_end_size;
extern unsigned int	state_machine_table_size;
extern unsigned int out_file_uncompfunc_size;


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  output_table
 *  Description:  This function will build the compressed table, the mask table and
 *-------------------------------------------------------------------------------------*/
void	output_table ( MASK_TABLE* table, int output_file)
{
	int				size;
	char			string[1024];
	unsigned int	count;
	unsigned int	index;
	unsigned int	failed = 0;
	unsigned char*	comp_row;
	unsigned char*	mask_size;
	unsigned char*	comp_size;

	if (table->num_masks > UINT_MAX)
		failed = 1;
	else if (table->num_masks > USHRT_MAX)
		mask_size = "unsigned int";
	else if (table->num_masks > UCHAR_MAX)
		mask_size = "unsigned short";
	else
		mask_size = "unsigned char";

	if (table->num_comp_masks > UINT_MAX)
		failed = 1;
	else if (table->num_comp_masks > USHRT_MAX)
		comp_size = "unsigned int";
	else if (table->num_comp_masks > UCHAR_MAX)
		comp_size = "unsigned short";
	else
		comp_size = "unsigned char";

	/* dump the state table */
	size = sprintf(string,"typedef struct\n{\t%s\tmask;\n\t%s\ttable;\n} LOOKUP_TABLE;\n\n",mask_size,comp_size);
	write(output_file,string,size);

	size = sprintf(string,"static LOOKUP_TABLE\tstate_table[%d] = {",table->num_rows);
	write(output_file,string,size);

	for (count=0;count<table->num_rows;count++)
	{
		if (count%16 == 0)
		{
			size = sprintf(string,"\n\t");
			write(output_file,string,size);
		}

		size = sprintf(string,"{%2d,%2d}%c",table->row_map[count],table->comp_map[count],(count==table->num_rows-1)?'}':',');
		write(output_file,string,size);
	}

	size = sprintf(string,";\n\n");
	write(output_file,string,size);
	
	/* dump the symbol mask table */
	if (table->entry_size == 1)
	{
		size = sprintf(string,"static unsigned int mask_table[%d] = {",table->num_masks);
		write(output_file,string,size);

		for (count=0;count<table->num_masks;count++)
		{
			if (count%8 == 0)
			{
				size = sprintf(string,"\n\t");
				write(output_file,string,size);
			}
				
			size = sprintf(string,"0x%08x%c",table->mask_array[count],(count==table->num_masks-1)?'}':',');
			write(output_file,string,size);
		}
		size = sprintf(string,";\n\n");
		write(output_file,string,size);
	}
	else
	{
		size = sprintf(string,"static unsigned int mask_table[%d][%d] = {",table->num_masks,table->entry_size);
		write(output_file,string,size);

		for (count=0;count<table->num_masks;count++)
		{
			size = sprintf(string,"\t{");
			write(output_file,string,size);

			for (index=0;index<table->map_size;index++)
			{
				size = sprintf(string,"0x%08x%c",table->mask_array[count*table->entry_size+index],(index==table->entry_size-1)?'}':',');
				write(output_file,string,size);
			}
			size = sprintf(string,"%c\n",(count==table->num_masks-1)?'}':',');
			write(output_file,string,size);
		}
		size = sprintf(string,";\n\n");
		write(output_file,string,size);
	}

	/* dump the actual next state table */
	size = sprintf(string,"static %s table[%d][%d] = {",comp_size,table->num_comp_masks,table->row_size);
	write(output_file,string,size);

	for (count=0;count<table->num_comp_masks;count++)
	{
		size = sprintf(string,"\n\t{");
		write(output_file,string,size);

		comp_row = &table->comp_table[count * table->row_size];
			
		size = sprintf(string,"0x%02x",comp_row[0]);
		write(output_file,string,size);

		for(index = 1; index <table->row_size; index++)
		{
			size = sprintf(string,",0x%02x",comp_row[index]);
			write(output_file,string,size);
		}

		size = sprintf(string,"}%c",(count==table->num_comp_masks-1)?'}':',');
		write(output_file,string,size);
	}

	size = sprintf(string,";\n");
	write(output_file,string,size);
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  build_output
 *  Description:  This function will write the array of data in a format that can be
 *  			  compiled by a C compiler.
 *-------------------------------------------------------------------------------------*/
void	build_output(	char* 			output_name,
						char*			header_dir,
						char*			enum_prefix,
						char* 			table,
						char*			look_uptable,
						unsigned int	num_rows,
						unsigned int 	num_symbols,
						char**			word,
						unsigned int*	word_size, 
						unsigned int	num_of_words,
						unsigned int	ignore_case,
						MASK_TABLE*		mask_table,
						unsigned int	uncompressed_table)
{
	int	count;
	int index;
	int file_dot;
	int out_file;
	char	*base;
	char	uppercase[255];
	char	lowercase[255];
	char	outfile_name[255];

	file_dot = strlen(output_name);
	memcpy(outfile_name,output_name,file_dot);

	outfile_name[file_dot] = '.';
	outfile_name[file_dot+1] = 'c';
	outfile_name[file_dot+2] = '\0';

	base = basename(output_name);

	for (index=0;base[index] != '\0' && index < 255;index++)
	{
		lowercase[index] = base[index];
		uppercase[index] = toupper(base[index]);
	}
	lowercase[index] = '\0';
	uppercase[index] = '\0';

	out_file = open(outfile_name,O_WRONLY|O_CREAT|O_TRUNC,S_IWRITE|S_IREAD);

	if (out_file != -1)
	{
		char string[1024];
		int size;

		/* write the file header */
		write(out_file,out_file_header,out_file_header_size-1);
		size = sprintf(string,"#include \"%s.h\"\n\n",lowercase);
		write(out_file,string,size);

		/* Write out the list of strings */
		size = sprintf(string,"%s_STRING_TABLE\t%s_table[%d] = {\n",uppercase,lowercase,num_of_words);
		write(out_file,string,size);
			
		size = sprintf(string,"\t\t{\"%s\",%d}",word[0],word_size[0]);
		write(out_file,string,size);
		
		for (count=1;count<num_of_words;count++)
		{
			size = sprintf(string,",\n\t\t{\"%s\",%d}",word[count],word_size[count]);
			write(out_file,string,size);
		}
		write(out_file,"\n};\n\n",5);

		/* Dont need the symbol table look-up for the uncompressed tables */
		if (uncompressed_table)
		{
	
			/* now write the look up tables */
			size = sprintf(string,"static unsigned char\tstate_machine_table[%d][%d] = {",num_rows,num_symbols);
			write(out_file,string,size);

			for (count=0;count<(num_rows*num_symbols);count += num_symbols)
			{
				if (count == 0)
					write(out_file,"\n\t\t\t\t\t\t\t\t",9);
				else
					write(out_file,",\n\t\t\t\t\t\t\t\t",10);

				write_group(out_file,&table[count],num_symbols,"\t\t\t\t\t\t\t\t ",9);
			}
			write(out_file,"};\n",3);

			write(out_file,out_file_start,out_file_start_size-1);
			write(out_file,lowercase,index);
			
			write(out_file,out_file_uncompfunc,out_file_uncompfunc_size-1);
		}
		else
		{
			/* now write the symbol conversion table */
			size = sprintf(string,"static signed char\tsymbol_table[%d] = \n\t",ALPHABET_SIZE);
			write(out_file,string,size);
			write_group(out_file,look_uptable,ALPHABET_SIZE,"\t ",2);
			write(out_file,";\n\n",3);

			/* write the rest of the output tables */
			output_table(mask_table,out_file);

			/* write the search function */
			write(out_file,out_file_start,out_file_start_size-1);
			write(out_file,lowercase,index);
			write(out_file,out_file_function,out_file_function_size-1);
		}

		if (ignore_case)
		{
			size = sprintf(string,ignore_case_compare,lowercase);
		}else{
			size = sprintf(string,case_compare,lowercase);
		}
		write(out_file,string,size);

		write(out_file,lowercase,index);

		write(out_file,case_compare_end,case_compare_end_size-1);

		close(out_file);
	}

	if (header_dir == NULL)
	{
		outfile_name[file_dot] = '.';
		outfile_name[file_dot+1] = 'h';
		outfile_name[file_dot+2] = '\0';
	}
	else
	{
		file_dot = strlen(header_dir);
		memcpy(outfile_name,header_dir,file_dot);
		memcpy(&outfile_name[file_dot],lowercase,index);
	
		outfile_name[file_dot+index  ] = '.';
		outfile_name[file_dot+index+1] = 'h';
		outfile_name[file_dot+index+2] = '\0';
	}
		
	out_file = open(outfile_name,O_WRONLY|O_CREAT|O_TRUNC,S_IWRITE|S_IREAD);

	if (out_file != -1)
	{
		char string[1024];
		int size;

		write(out_file,out_file_header,out_file_header_size-1);

		size = sprintf(	string,
						"#ifndef\t__%s_H__\n#define __%s_H__\n\ntypedef\tstruct\n{\n\tchar*\t\t\tname;\n\tunsigned int\tlength;\n} %s_STRING_TABLE;\n\n",
						uppercase,
						uppercase,
						uppercase);
		write(out_file,string,size);

		write(out_file,"typedef enum\n{\n",sizeof("typedef enum\n{")-1);

		for (count=0;count<num_of_words;count++)
		{
			for (index = 0; word[index] != '\0'; index++)
			{
				word[count][index] = toupper(word[count][index]);

				if (word[count][index] == '-')
					word[count][index] = '_';
			}

			if (count == 0)
				size = sprintf(string,"\n\t%s%s",enum_prefix,word[count]);
			else
				size = sprintf(string,",\n\t%s%s",enum_prefix,word[count]);

			write(out_file,string,size);
		}
			
		/* put the MAX at the end of the list */
		size = sprintf(string,",\n\t%sNUM_STRINGS",enum_prefix);
		write(out_file,string,size);

		size = sprintf(	string,"\n} %s%s;\n\n",enum_prefix,uppercase);
		write(out_file,string,size);

		size = sprintf(	string,"int\t%s_check_word(unsigned char* word);\n",lowercase);
		write(out_file,string,size);

		write(out_file,"\n#endif\n\n",sizeof("#endif\n\n")-1);
		close(out_file);
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  write_group
 *  Description:  This function will write the array of data in a format that can be
 *  			  compiled by a C compiler.
 *-------------------------------------------------------------------------------------*/
void	write_group(int out_file,char* buffer,unsigned int size,char* line_prefix,unsigned int prefix_size)
{
	int	index = 0;
	int count;
	int kount;
	int position;
	char	out_buffer[5*16+7];
	char	hex_char[] = "0123456789ABCDEF";

	write(out_file,"{",1);

	/* now write the lookup table to the file */
	for (count=0;count<16 && index < size;count++)
	{
		position = 0;

		for (kount=0;kount<16 && index < size;kount++,index++)
		{
			out_buffer[position++] = '0';
			out_buffer[position++] = 'x';
			out_buffer[position++] = hex_char[(buffer[index] & 0xf0) >> 4];
			out_buffer[position++] = hex_char[(buffer[index] & 0x0f)];
			out_buffer[position++] = ',';
		}
		
		if (index < size)
		{
			out_buffer[position++] = '\n';
		}else{
			position--;
			out_buffer[position++] = '}';
		}
			
		write(out_file,out_buffer,position);

		if (index < size)
			write(out_file,line_prefix,prefix_size);
	}
}


