

 char* zmiana_na_stringa1(int lidzba, char *dane)
{


	if (lidzba % 10 != 0)
	{
		dane[7] = (lidzba % 10) + 48;

	}
	if (lidzba % 100 != 0)
	{
		dane[6] = (lidzba % 100) / 10 + 48;
	}
	if (lidzba % 1000 != 0)
	{
		dane[5] = (lidzba % 1000) / 100 + 48;
	}

	return dane;
}