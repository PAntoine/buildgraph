#include "output.h"
extern OUTPUT_STRING_TABLE	output_table[121];
main()
{
	int count;
	int res;
	int offset = 0;

	for (count=0;count<121;count++)
	{
		if (count != (res = output_check_word(output_table[count].name)))
			printf("Failed! %d %d\n",offset,res);

//		if (count == (res = output_check_word("pacoraban")))
//			printf("worked! %d %d\n",offset,res);
//		else
//			printf("Failed! %d %d\n",offset,res);


	}
}
