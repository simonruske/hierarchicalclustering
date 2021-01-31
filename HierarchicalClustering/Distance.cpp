float SquaredEuclidean(float* data, int firstIndex, int secondIndex, int numberOfColumns)
{
	float distance = 0;
	for (int i = 0; i < numberOfColumns; i++)
	{
		float difference = data[firstIndex * numberOfColumns + i] - data[secondIndex * numberOfColumns + i];
		distance += difference * difference;
	}
	return distance;
}