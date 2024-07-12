import numpy as np
import pytest
from example import add

@pytest.mark.parametrize(
    "value_a,value_b",
    [
        (1, np.array([1, 2, 3])),
        (np.array([1, 2, 3]), 1),
        (1, 1),
    ]
)
def test_invalid_arguments_raises_type_error(value_a, value_b):
    with pytest.raises(
        TypeError, 
        match="Input arguments must be numpy arrays."
    ):
        
        add(value_a, value_b)


def test_non_matching_sizes_raises_value_error():

    with pytest.raises(
        ValueError,
        match="Arrays must be of the same size.",
    ):
        add(np.array([1, 2, 3]), np.array([1, 2, 3, 4]))

def test_two_different_numpy_arrays_adds_correctly():

    #Arrange
    array_1 = np.array([1, 2, 3])
    array_2 = np.array([2, 3, -1])

    #Act
    result = add(array_1, array_2)

    #Assert
    np.testing.assert_almost_equal(
        result, 
        np.array([3, 5, 2]),
    )
