/***************************************************************************************
 *
 *       Filename:  build_graph.h
 *
 *    Description:  This file holds the header file for the build graph code.
 *
 *        Version:  1.0
 *        Created:  31/03/2009 10:40:30
 *       Revision:  none
 *
 *         Author:  Peter Antoine
 *          Email:  me@peterantoine.me.uk
 *
 *------------------------------------------------------------------------------------- 
 *                         Copyright (c) 2009 : Peter Antoine
 *                        Released under the Artistic License.
 ***************************************************************************************/

#ifndef  __BUILD_GRAPH_H__
#define  __BUILD_GRAPH_H__

#define	__BG_VERSION__ "0.6"
#define	ALPHABET_SIZE	(256)

/*-----------------------------------------------------------------------------*
 *  Types required for the application
 *-----------------------------------------------------------------------------*/
typedef struct tag_n_node
{
	unsigned int		dumped;
	unsigned int		enumeration;
	unsigned int		number_on_branch;
	struct tag_n_node	*branches[ALPHABET_SIZE];
} N_NODE;

typedef struct 
{
	unsigned int	entry_size;
	unsigned int	map_size;
	unsigned int	row_size;
	unsigned int	num_masks;
	unsigned int	num_comp_masks;
	unsigned int	num_rows;
	unsigned int*	row_map;
	unsigned int*	comp_map;
	unsigned int*	mask_array;
	unsigned int*	comp_mask_array;
	unsigned char*	comp_table;

} MASK_TABLE;

/*-----------------------------------------------------------------------------*
 *  Function Protptypes.
 *-----------------------------------------------------------------------------*/
void			decase_table (unsigned char* table,unsigned int num_rows,unsigned int row_size);
unsigned int	count_words (char* buffer, unsigned int buffer_size);
void			write_group(int out_file,char* buffer,unsigned int size,char* line_prefix,unsigned int prefix_size);
void 			build_dictionary (unsigned int num_of_words,char *buffer, unsigned int buffer_size,char** word,unsigned int* word_size,int ignore_case);
void 			build_naive_tree (N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words);
unsigned int	prune_tree (N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words);
void 			generate_naive_table (char* table,N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words);
unsigned int	compress_table (char* table,char* look_uptable,char** compressed_table,unsigned int	num_branches,char** word,unsigned int* word_size, unsigned int num_of_words,unsigned int ignore_case);
int				uncompressed_check_word(char* table,char* string,unsigned int ignore_case,char** word,unsigned int* word_size);
int				compressed_check_word(char* compressed_table, char* symbol_table, unsigned int num_symbols, char* string, unsigned int ignore_case, char** word, unsigned int* word_size); 
void			output_table ( MASK_TABLE* table, int output_file);
void	build_output(	char* 			output_name,
						char*			enum_prefix,
						char* 			table,
						char*			look_uptable,
						unsigned int	num_rows,
						unsigned int 	num_symbols,
						char**			word,
						unsigned int*	word_size, 
						unsigned int	num_of_words,
						unsigned int	ignore_case,
						MASK_TABLE* 	mask_table,
						unsigned int	uncompressed_table);

MASK_TABLE*	build_mask_table ( unsigned char* table,unsigned int num_rows,unsigned int row_size);
void	calculate_overlays (  unsigned char* table,unsigned int num_rows,unsigned int row_size, unsigned char** overlaid_table, unsigned int* overlaid_rows );


#endif 

/* $Id: build_graph.h,v 1.2 2009/04/16 14:51:54 PeterAntoine Exp $ */

