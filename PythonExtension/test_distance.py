
from example import distance
import numpy as np


def test_example_numpy_array_calculates_distance_correctly():

    #Arrange
    array = np.array([
        [1, 0, -1],
        [5, 5, 5],
    ])

    #Act
    result = distance(array)

    #Assert
    assert result == 77.0