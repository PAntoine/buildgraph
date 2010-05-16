#include "ical_strings.h"
extern ICAL_STRINGS_STRING_TABLE	ical_strings_table[119];

main()
{
	int count;
	int res;
	int offset = 0;

	for (count=0;count<119;count++)
	{
		if (count == (res = ical_strings_check_word(ical_strings_table[count].name)))
			printf("worked! %d %d\n",offset,res);
		else
			printf("Failed! %d %d\n",offset,res);

		if (count == (res = ical_strings_check_word("pacoraban")))
			printf("worked! %d %d\n",offset,res);
		else
			printf("Failed! %d %d\n",offset,res);


	}
}
