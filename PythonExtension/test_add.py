import pytest
from example import add

@pytest.mark.parametrize(
    "value_a,value_b,expected",
    [
        (1,2,3),
        (1,-1,0),
    ]
)
def test_add(value_a, value_b, expected):
    assert add(value_a, value_b) == expected