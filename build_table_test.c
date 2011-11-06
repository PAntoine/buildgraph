#include "output.h"
extern OUTPUT_STRING_TABLE	output_table[TST_NUM_STRINGS];
main()
{
	int count;
	int res;
	int offset = 0;
	int num_failed = 0;

	for (count=0;count<TST_NUM_STRINGS;count++)
	{
		if (count != (res = output_check_word(output_table[count].name)))
		{
			num_failed++;
			printf("Failed! %d %d (item: %d name: %s)\n",offset,res,count,output_table[count].name);
		}
	}

	if ((res = output_check_word("pacoraban")) > 0)
		printf("Failed! %d %d\n",offset,res);

	printf("%d strings tested %d failed.\n",count,num_failed);
}
