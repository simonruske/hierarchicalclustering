from mymodules import PyQueue

def test_queue_example():

    #Arrange
    q = PyQueue()
    q.add_to_array1(10)
    q.add_to_array1(20)

    #Act
    value_1 = q.get_value_in_array1(0)
    value_2 = q.get_value_in_array1(1)
    value_3 = q.get_value_in_array1(2)
    
    #Assert
    assert value_1 == 10
    assert value_2 == 20
    assert value_3 == 0