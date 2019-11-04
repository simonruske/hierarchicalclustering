#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include <cstdlib>

void swap(int *queue, int elem_1, int elem_2)
{
    // Function that swaps two elements of a vector

    int temp = queue[elem_1];
    queue[elem_1] = queue[elem_2];
    queue[elem_2] = temp;
}

int bubble_up(float *vec, int *queue, int elem)
{
    // Function that swaps an element with its parent and returns its new location

    if(elem == 1)  // If already at the top then stop
        return 1;

    int parent = elem / 2;

    if(vec[queue[parent]] > vec[queue[elem]]) // If Parent is bigger than child swap
    {
        swap(queue, parent, elem);
        return parent;
    }
    return elem;

}

int bubble_down(float *vec, int *queue, int elem)
{
    // Function that swaps an element with its smallest child and returns its new location
    int length = queue[0] + 1;
    int l_idx = 2 * elem;
    int r_idx = 2 * elem + 1;

    if(l_idx >= length) // If there are no children stop
        return elem; // Element stays where it was

    int m_idx = elem;

    if(vec[queue[m_idx]] > vec[queue[l_idx]]) // See if left child is smaller
    {
        m_idx = l_idx;
    }
    // If right child exists see if it is smaller
    if((r_idx < length) && (vec[queue[m_idx]] > vec[queue[r_idx]]))
        m_idx = r_idx;

    if(m_idx != elem) // If a child is smaller then swap
        swap(queue, elem, m_idx);

    return m_idx; // The parent is smaller than the children so stop
}

void fix(float *vec, int *queue, int elem)
{
    // Function that successively bubbles down an element as far as it will go.
    int cur_idx = elem;
    int prev_idx = elem;
    do
    {
        prev_idx = cur_idx;
        cur_idx = bubble_down(vec, queue, prev_idx);
    }
    while(prev_idx != cur_idx);
}

void heapify(float *vec, int *queue)
{
    // Function that turns a queue (of indices) into a heap using a vector
    // The vector stays the same and the queue changes depending on the elements of the vector
    for(int i = queue[0] / 2; i > 0; i--)
        fix(vec, queue, i);
}

int main()
{

    // Reads in the file

    std::cout << "Enter file name:" << std::endl;
    std::string file_name;
    std::cin >> file_name;

    std::ifstream myfile;
    myfile.open(file_name.c_str());
    if(myfile.is_open())
    {

        int m = 0;
        int n = 0;
        int final_n = 0;
        std::string line;
        while(std::getline(myfile, line))
        {
            n = 0;
            m++;
            std::stringstream ss(line);
            std::string item;
            while(std::getline(ss, item, ','))
            {
                n++;
            }
            // If final_n has not been set then set it
            if(final_n == 0)
            {
                final_n = n;
            }
            // Otherwise check that the new n is correct
            else if(final_n != n)
            {
                std::cout << "There was something wrong with the input file" << std::endl;
                break;
            }
        }

        // Reads file into array
        float vec[m*n];
        std::cout << "There are " << m << " rows and " << n << " columns in the input matrix." << std::endl;
        std::ifstream myfile;
        myfile.open(file_name.c_str());
        for(int i = 0; i < m; i++)
        {
            std::getline(myfile, line);
            std::stringstream ss(line);

            for(int j = 0; j < n; j++)
            {
                std::string token;
                std::getline(ss, token, ',');
                vec[n*i + j] = std::atof(token.c_str());
            }
        }
        // Calculates Intitial Nearest Neighbours
        std::clock_t begin = clock();
        float min_dist[2*m];
        int nn[2*m];
        std::cout << "Calculating Initial Nearest Neighbours" << std::endl;

        for(int i = 0; i < m-1; i++)
        {
            for(int j = i + 1; j < m; j++)
            {

                float dist = 0;
                int k = 0;

                for(int k = 0; k < n; k++)
                {
                    float diff = vec[i*n + k] - vec[j*n + k];
                    dist += diff * diff;
                }

                dist = std::sqrt(dist);

                if(j == i + 1)
                {
                     min_dist[i+1] = dist;
                     nn[i+1] = j+1;
                }

                else if(min_dist[i] > dist)
                {
                    min_dist[i+1] = dist;
                    nn[i+1] = j+1;
                }
            }
        }

        std::clock_t end = clock();
        std::cout << double(end-begin) / CLOCKS_PER_SEC << std::endl;

        std::cout << "Creating a queue from the nearest neightbours" << std::endl;
        begin = clock();
        // Poupulates queue and turns into heap
        int queue[m];
        queue[0] = m-1;
        for(int i = 1; i < m ; i++)
            queue[i] = i;

        heapify(min_dist, queue);
        end = clock();
        std::cout << double(end-begin) / CLOCKS_PER_SEC << std::endl;

        begin = clock();
        // Setup variables

        float linkage[(m-1)*3]; // The linkage matrix to return
        int loc[2*m]; // vector indicating where a cluster exists
        int sizes[2*m];

        // Populates location vector and sizes
        for(int i = 0; i <= m; i++)
        {
            loc[i] = i;
            sizes[i] = 1;
        }

        for(int i = m+1; i < 2*m; i++)
            loc[i] = 0;

        /*
        =========
        MAIN LOOP
        =========
        */

        std::cout << "MAIN LOOP" << std::endl;
        for(int depth = 0; depth < m-1; depth++)
        {
            std::cout << depth << " of " << m-1 << " merges complete" << std::endl;
            // Get the next a, b that exists
            int a = queue[1];
            int b = nn[a];

            while(loc[a] == 0 || loc[b] == 0)
            {
                if(loc[a] == 0) // If 'a' doesn't exist then delete it
                {
                    queue[1] = queue[queue[0]]; // Put the last element in the first space
                    queue[0]--; // Reduce the size of the queue by 1
                    fix(min_dist, queue, 1); // Updates the queue
                    a = queue[1]; // Get the next best a
                    b = nn[a];
                }

                // If 'a' has an out of date nearest neighbour then update it
                else if(loc[b] == 0)
                {
                    for(int i = 1; i < 2*m; i++)
                    {
                        if(loc[i] != 0) // if i exists
                        {
                            for(int j = i + 1; j < 2*m; j++)
                            {
                                bool flag = 1;
                                if(loc[j] !=0) // If j exist
                                {

                                    float dist = 0;
                                    int k = 0;

                                    for(int k = 0; k < n; k++)
                                    {
                                       float diff = vec[loc[i]*n + k] - vec[loc[j] * n + k];
                                       dist += diff * diff;
                                    }
                                    dist = std::sqrt(dist);

                                    if(flag) // if first j then update minimum distance
                                    {
                                        min_dist[i+1] = dist;
                                        nn[i+1] = j+1;
                                        flag = 0;
                                    }

                                    else if(min_dist[i] > dist) // otherwise update if minimum distance has changed.
                                    {
                                        min_dist[i+1] = dist;
                                        nn[i+1] = j+1;
                                    }
                                }
                             }
                         }
                    }
                }
            }

            // Write a, b, delta to the linkage matrix
            linkage[    depth * 3] = a;
            linkage[1 + depth * 3] = b;
            linkage[2 + depth * 3] = min_dist[loc[a]];

            // Update locations and sizes
            loc[a] = 0;
            loc[b] = 0;
            loc[m+1+depth] = b;
            sizes[m+1+depth] = sizes[a] + sizes[b];

            // Updates centre
            float centre[n];
            for(int i = 0; i < 5; i++)
            {
                centre[i] = (vec[a * n + i] * sizes[a] + vec[b * n + i] * sizes[b]) / sizes[m+1+depth];
                vec[b*n + i] = centre[i];
            }

            // calculates Nearest neighbours of the new centre
            bool flag = 1;
            for(int i = 1; i < 2*m; i++)
            {
                if(loc[i] != 0) // if i exists
                {
                    float dist = 0;
                    int k = 0;


                    for(int k = 0; k < n; k++)
                    {
                        float diff = vec[loc[i]*n + k] - vec[loc[m+1+depth] * n + k];
                        dist += diff * diff;
                    }
                    dist = std::sqrt(dist) * std::sqrt(2 * (sizes[m+1+depth] * sizes[i]) / (sizes[m+1+depth] + sizes[i])) * dist;
                    if(dist < min_dist[i])
                    {
                        min_dist[i] = dist;
                        nn[i] = m+1+depth;
                    }
                    if(flag = 1)
                    {
                        min_dist[m+1+depth] = dist;
                        flag = 0;
                    }
                    else if(dist < min_dist[m+1+depth])
                    {
                         min_dist[m+1+depth] = dist;
                         nn[m+1+depth] = i;
                    }
                }
             }

          // updates the queue

          queue[1] = m+1+depth;
          fix(min_dist, queue, 1);


        }
        end = clock();
        std::cout << double(end-begin) / CLOCKS_PER_SEC << std::endl;
        for(int i = 0; i < m-1; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                std::cout << linkage[i*n + j] << ", ";
            }
            std::cout << linkage[i*n + 2] << std::endl;
       }
    }

    else
    {
        std::cout << "Could not read file" << std::endl;

    }
}
