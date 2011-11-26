static const char encoding_string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

main()
{
	int	count;
	unsigned char	table[256];

	for (count=0;count<255;count++)
		table[count] = 0x00;

	for (count=0;count<sizeof(encoding_string);count++)
	{
		table[encoding_string[count]] = 1;
		printf("%c = %d\n",encoding_string[count],count);
	}

	table['='] = 0;

	for (count=0;count<256;count++)
	{
		printf("0x%02x,",table[count]);

		if ((count % 16) == 15)
			printf("\n");
	}
}


