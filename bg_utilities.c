/***************************************************************************************
 *
 *       Filename:  bg_utilities.c
 *
 *    Description:  This file holds the utilities that make up the graph builder
 *                  and more impotently can be used in other applications to make
 *                  an on the fly lex type parser.
 *
 *        Version:  1.0
 *        Created:  28/03/2009 10:40:30
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
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  count_words
 *  Description:  This function will return the number of "words" that are found in 
 *                a block of memory. Be careful the number of words really mean the
 *                number of lines in the memory, to the number of symbols that need
 *                to be searched for.
 *-------------------------------------------------------------------------------------*/
unsigned int	count_words (char* buffer, unsigned int buffer_size)
{
	unsigned int	count;
	unsigned int	result = 0;

	for (count=0;count<buffer_size;count++)
	{
		/* am looking for a "\n" "\r\n" as the line endings, everthing
		 * between these are assumed to be words.
		 */
		if (buffer[count] == '\n')
			result++;
	}

	return result;
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  build_dictionary
 *  Description:  This function will take a block of memory and build a dictionary
 *                of words from that data. But, the data is not really words but
 *                line-ended symbols.
 *-------------------------------------------------------------------------------------*/
void build_dictionary (unsigned int num_of_words,char *data, unsigned int buffer_size,char** word,unsigned int* word_size,int ignore_case,int quiet)
{
	int	count;
	int	size;
	int	number = 0;

	word[0] = data;

	/* find the index and word sizes */
	for (count=0,size=0;count<buffer_size;count++,size++)
	{
		if (data[count] == '\r' || data[count] == '\n')
		{
			data[count] = 0;

			if (data[count-1] != 0)
			{
				word_size[number] = size;
				number++;
			}
			size = -1;
			word[number] = &data[count+1];
		}

		/* convert all chars to upper case, see later for the clever bit.
		 * Think about it, this will cause a problem. You have until the
		 * lower bit of code to work out what that is.
		 */
		if (ignore_case)
		{
			data[count] = (toupper(data[count]));
		}
	}

	if (!quiet)
	{
		printf("About to build a parse tree for the following words:\n");
		for (count = 0;count < num_of_words;count++)
		{
			printf("word[%3d] %s size: %d\n",count,word[count],word_size[count]);
		}
	}
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  build_naive_tree
 *  Description:  This function will build the first naive tree.
 *-------------------------------------------------------------------------------------*/
void build_naive_tree (N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words)
{
	unsigned int 	count;
	unsigned int	letter;
	N_NODE*			current;

	for (count=0;count<num_of_words;count++)
	{
		current = head_node;

		for (letter=0;letter <= word_size[count];letter++)
		{
			if (current->branches[word[count][letter]] == NULL)
			{
				/* add a first node */
				current->branches[word[count][letter]] = (N_NODE*) malloc(sizeof(N_NODE));
				memset((char*)current->branches[word[count][letter]],0,sizeof(N_NODE));
			}

			current = current->branches[word[count][letter]];
			
			current->number_on_branch++;
		}
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  prune_tree
 *  Description:  This function will handle the tree pruning by walking the tree
 *                again and marking where it has been. It stops when it reaches a node
 *                leaf node.
 *                This marks the branches that are not leaf nodes. It also counts how
 *                many branches that we have that that sub-tree.
 *-------------------------------------------------------------------------------------*/
unsigned int	prune_tree (N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words)
{
	unsigned int	count;
	unsigned int	letter;
	unsigned int	item = 0;
	N_NODE*			current;

	for (count=0;count<num_of_words;count++)
	{
		current = head_node;

		for (letter=0;letter< word_size[count];letter++)
		{
			/* OK, only one item on the branch, we know it's a leaf node */
			if (current->branches[word[count][letter]] != NULL && current->branches[word[count][letter]]->number_on_branch == 1)
			{
				break;
			}

			current = current->branches[word[count][letter]];
		
			/* give the branch a number */
			if (current->enumeration == 0)
			{
				current->enumeration = item++;
			}

			/* cheap check to make sure the sub tree is not also a leaf */
			if (current->number_on_branch == 1)
			{
				break;
			}
		}

		if (letter == word_size[count])
		{
			current->enumeration = item++;
		}
	}

	return item;
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  generate_naive_table
 *  Description:  This function will create the table from the pruned tree. Note this
 *                table will be as wide as the alphabet used (255 in the case) so there
 *                is a lot of redundancy that can be removed.
 *
 *                NOTE: I should be able to do this when I am pruning the tree, I
 *                      think?
 *-------------------------------------------------------------------------------------*/
void generate_naive_table (char* table,N_NODE* head_node,char** word,unsigned int* word_size, unsigned int num_of_words)
{
	unsigned int 	count;
	unsigned int 	letter;
	N_NODE*			current;

	for (count=0;count<num_of_words;count++)
	{
		current = head_node;

		for (letter=0;letter < word_size[count];letter++)
		{
			if (current->branches[word[count][letter]] != NULL)
			{
				table[(current->enumeration * ALPHABET_SIZE) + word[count][letter]] = current->branches[word[count][letter]]->enumeration;
			}

			if (current->branches[word[count][letter]] != NULL && current->branches[word[count][letter]]->number_on_branch == 1)
			{
				table[(current->enumeration * ALPHABET_SIZE) + word[count][letter]] = 0 - (count + 1);
				break;
			}

			current = current->branches[word[count][letter]];
		}

		if (letter == word_size[count])
		{
			table[(current->enumeration * ALPHABET_SIZE) + word[count][letter]] = 0 - (count + 1);
		}
	}
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  compress_table
 *  Description:  This function will generate a table with a reduced alphabet to 
 *                reduce the amount of memory required for the finished table. There
 *                is a requirement for a conversion function (lookup) that will 
 *                convert from the full alphabet to the reduced one used in the tables.
 *
 *                For each letter search there is a table lookup, to handle the smaller
 *                alphabet. But, this is an extra lookup per letter searched. This may
 *                be an unnecessary overhead. But, having bigger tables is more likely
 *                to create a cache miss that is more costly than the lookup overhead
 *                (as this is always likely to be in the cache - so really cheap).
 *
 *                There is no instant win here, you will have to test.
 *
 *                NOTE: There is an extra compression missed here. I am not pruning the 
 *                      symbols used. I should keep a count of the all symbols in the
 *                      pruned tree that actually get used. I should be able to make
 *                      the compressed tables smaller. Also, the first letter lookup
 *                      should end the search! That maybe the problem I was having and
 *                      had to add the +1 hack!
 *
 *                NOTE: Just thought, can compress even harder. We can branch collision
 *                      test. If we have symbols on a branch that is not used on another
 *                      then we could use the previously unused symbol and we should be
 *                      able to remove some of the branches. Especially the branches 
 *                      that a close to the end of the tree. We should start with the
 *                      branches that have the fewest entries and work backwards.
 *-------------------------------------------------------------------------------------*/
unsigned int	compress_table(	char*			table,
								char*			look_uptable,
								char**			compressed_table,
								unsigned int	num_branches,
								char**			word,
								unsigned int*	word_size, 
								unsigned int	num_of_words,
								unsigned int	ignore_case)
{
	unsigned int	count;
	unsigned int	kount;
	unsigned int	letter;
	unsigned int	num_symbols = 0;
	unsigned char*	temp_array;

	for (count=0;count<num_of_words;count++)
	{
		for (letter=0;letter < word_size[count];letter++)
		{
			if (look_uptable[word[count][letter]] == 0)
			{
				num_symbols++;
				look_uptable[word[count][letter]] = num_symbols;
			}

			if (ignore_case)
			{
				/* Solution: Make the lower-case letters in the look up table point to the
				 * toupper'ed ones from the top.
				 * I wont explain why it has to be done this way, but it should be obvious.
				 */
				look_uptable[tolower(word[count][letter])] = look_uptable[word[count][letter]];
			}

		}
	}

	temp_array = (unsigned char*) malloc(num_branches * num_symbols);
	memset(temp_array,0,(num_branches * num_symbols));

	for (kount=0;kount<num_branches;kount++)
	{
		for (count=0;count<ALPHABET_SIZE;count++)
		{
			if (table[(kount*ALPHABET_SIZE) + count] != 0)
				temp_array[(kount*num_symbols) + look_uptable[count]] = table[kount*ALPHABET_SIZE + count];
		}
	}

	*compressed_table = temp_array;

	return num_symbols;
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  free_tree
 *  Description:  This is a crap recursive tree walk to release the memory.
 *                I could write a proper one, but why bother when you can let the stack
 *                do the work for you. :)
 *-------------------------------------------------------------------------------------*/

void free_tree (N_NODE* tree)
{
	int count = 0;

	for (count=0;count<ALPHABET_SIZE;count++)
	{
		if (tree->branches[count] != NULL)
		{
			free_tree(tree->branches[count]);
			free(tree->branches[count]);
			tree->branches[count] = NULL;
		}
	}
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  compressed_check_word
 *  Description:  This function is not required for the build tree but is a version
 *                of the auto-generated code that is used for searching the table
 *                that has been produced.
 *
 *                This is here for library uses that want to search the table that has
 *                been generated and does not want to go through a compile cycle.
 *-------------------------------------------------------------------------------------*/
int	compressed_check_word(	char* 			compressed_table,
							char* 			symbol_table,
							unsigned int 	num_symbols,
							char* 			string,
							unsigned int 	ignore_case,
							char** 			word,
							unsigned int* 	word_size)
{
	int	line = 0;
	int	count = 0;
	int letter = 0;

	do
	{
		line = compressed_table[(line*num_symbols) + symbol_table[string[letter]]];
		letter++;
	}
	while (line > 0 && string[letter] != '\0');

	if (line < 0)
		line = (0-line) - 1;
	else if ((line = compressed_table[(line*num_symbols) + symbol_table[0]]) < 0)
		line = (0-line) - 1;
	else
		line = -1;

	/* this bit does the compare against the real strings after the cleaver
	 * search has completed.
	 *
	 * Its the bit that cant be optimised :(
	 */
	if (line >= 0)
	{
		if (ignore_case)
		{
			for (count=letter;count < word_size[line];count++)
			{
				if (toupper(string[count]) != word[line][count])
				{
					line = -1;
					break;
				}
			}
		}else{
			for (count=letter;count < word_size[line];count++)
			{
				if (string[count] != word[line][count])
				{
					line = -1;
					break;
				}
			}
		}
	}
	
	return line;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  uncompressed_check_word
 *  Description:  This function is not required for the build tree but is a version
 *                of the auto-generated code that is used for searching the table
 *                that has been produced.
 *
 *                This is here for library uses that want to search the table that has
 *                been generated and does not want to go through a compile cycle.
 *-------------------------------------------------------------------------------------*/
int	uncompressed_check_word(char* table,char* string,unsigned int ignore_case,char** word,unsigned int* word_size)
{
	int	line = 0;
	int	count = 0;
	int letter = 0;

	do
	{
		line = table[(line*ALPHABET_SIZE) + string[letter]];
		letter++;

	}
	while (line >= 0 && string[letter] != '\0');

	if (line < 0)
		line = (0-line) - 1;
	else if ((line = table[line*ALPHABET_SIZE]) < 0)
		line = (0-line) - 1;
	else
		line = -1;

	/* this bit does the compare against the real strings after the cleaver
	 * search has completed.
	 *
	 * Its the bit that cant be optimised :(
	 */
	if (line >= 0)
	{
		if (ignore_case)
		{
			for (count=letter;count < word_size[line];count++)
			{
				if (toupper(string[count]) != word[line][count])
				{
					line = -1;
					break;
				}
			}
		}else{
			for (count=letter;count < word_size[line];count++)
			{
				if (string[count] != word[line][count])
				{
					line = -1;
					break;
				}
			}
		}
	}
	
	return line;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  decase_table
 *  Description:  This function will make the table case independent. This is basically
 *  			  done by copying any entry that is not zero to its lower case match.
 *  			  As the case independent tables are formed in upper case (this just
 *  			  made the code easier). This will just convert down to lower case.
 *-------------------------------------------------------------------------------------*/
void	decase_table (unsigned char* table,unsigned int num_rows,unsigned int row_size)
{
	unsigned int	row_count;
	unsigned char 	count;
	unsigned char*	row;
	
	for (row_count=0;row_count<num_rows;row_count++)
	{
		row = &table[row_count * row_size];

		for(count = 'A'; count < 'Z'; count++)
		{
			row[tolower(count)] = row[count];
		}
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  build_mask_table
 *  Description:  This function will build the compressed table, the mask table and
 *                the compressed table that it requires to do the lookup.
 *                The structure returned has all the data required to build the 
 *                compressed output.
 *-------------------------------------------------------------------------------------*/
MASK_TABLE*	build_mask_table ( unsigned char* table,unsigned int num_rows,unsigned int row_size) 
{
	unsigned int	count;
	unsigned int	kount;
	unsigned int	xount;
	unsigned int	index;
	unsigned int	row_count;
	unsigned int	result = 0;
	unsigned int	map_size = (row_size + 31) / 32;	/* in dwords */
	unsigned char*	row;
	unsigned char*	comp_row;
	MASK_TABLE*		mask_table;

	mask_table = calloc(1,sizeof(MASK_TABLE));

	mask_table->entry_size = map_size;
	mask_table->num_rows = num_rows;
	mask_table->row_size = row_size;
	mask_table->num_masks = 0;
	mask_table->num_comp_masks = 0;
	mask_table->row_map = calloc(num_rows+1,sizeof(unsigned int));
	mask_table->comp_map = calloc(num_rows+1,sizeof(unsigned int));
	mask_table->comp_table = calloc(num_rows,row_size);
	mask_table->mask_array = calloc(map_size*4,num_rows);	/* max size */
	mask_table->comp_mask_array = calloc(map_size*4,num_rows); 	/* max size */

	for (row_count=0;row_count<num_rows;row_count++)
	{
		row = &table[row_count * row_size];

		for(kount=0;kount<map_size;kount++)
		{
			mask_table->mask_array[((mask_table->num_masks)*map_size)+kount] = 0;
		}

		/* find the symbols that matter in this row */
		for(count = 0; count < row_size; count++)
		{
			if (row[count] != 0)
			{
				mask_table->mask_array[((mask_table->num_masks)*map_size) + (count>>5)] |= (0x1 << (count & 0x1F));
			}
		}

		/* now check to see if this map already exists */
		for (xount=0;xount<mask_table->num_masks;xount++)
		{
			for(kount=0;kount<map_size;kount++)
			{
				if (mask_table->mask_array[((mask_table->num_masks)*map_size)+kount] != mask_table->mask_array[xount*map_size+kount])
				{
					break;
				}
			}

			if (kount == map_size)
			{
				/* found a match */
				mask_table->row_map[row_count] = xount;
				break;
			}
		}

		/* check if it needs adding to the map table */
		if (xount == mask_table->num_masks)
		{
			/* did not find a match */
			mask_table->row_map[row_count] = mask_table->num_masks;
			mask_table->num_masks++;
		}
	}

	/* now compress the tables, use the masks against the actual rows */
	for (count=0;count<num_rows;count++)
	{
		for (xount=0;xount<mask_table->num_comp_masks;xount++)
		{
			for(kount=0;kount<map_size;kount++)
			{
				if ((mask_table->mask_array[(mask_table->row_map[count]*map_size)+kount] & mask_table->comp_mask_array[xount*map_size+kount]) != 0)
				{
					break;
				}
			}

			if (kount == map_size)
			{
				/* we have found a safe overlay */
				for (kount=0;kount<map_size;kount++)
				{
					mask_table->comp_mask_array[(xount*map_size)+kount] |= mask_table->mask_array[mask_table->row_map[count]*map_size+kount];
				}

				row = &table[count * row_size];
				comp_row = &mask_table->comp_table[xount * row_size];

				for(index = 0; index < row_size; index++)
				{
					if (row[index] != 0)
					{
						comp_row[index] = row[index];
					}
				}

				mask_table->comp_map[count] = xount;
				break;
			}
		}

		if (xount == mask_table->num_comp_masks)
		{
			/* did not find one, need to add it to the end */
			for (kount=0;kount<map_size;kount++)
			{
				mask_table->comp_mask_array[(xount*map_size)+kount] = mask_table->mask_array[mask_table->row_map[count]*map_size+kount];
			}

			mask_table->comp_map[count] = mask_table->num_comp_masks;
			mask_table->num_comp_masks++;

			row = &table[count * row_size];
			comp_row = &mask_table->comp_table[xount * row_size];

			for(index = 0; index < row_size; index++)
			{
				if (row[index] != 0)
				{
					if (comp_row[index] != 0)
						printf("%d: error\n",index);

					comp_row[index] = row[index];
				}
			}
		}
	}

	return mask_table;
}


