unsigned power(unsigned base, unsigned exponent)
{
	if (exponent == 0U) {
		return 1;
	}
	return base * power(base, exponent-1);
}
