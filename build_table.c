static const char encoding_string[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

main()
{
	int	count;
	unsigned char	table[256];

	for (count=0;count<255;count++)
		table[count] = 0x00;

	for (count=0;count<64;count++)
	{
		table[encoding_string[count]] = count;
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


