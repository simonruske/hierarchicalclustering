#!/usr/bin/env python

import numpy as np
from scipy.spatial.distance import euclidean
from heapq import heapify, heappush
import tau

class Queue():

  def __init__(self):
    self.pq = []

  def argmin(self):
    return self.pq[0]

  def remove_min(self):
    queue_item = self.pq.pop(0)

    return(queue_item)

  def remove_elem(self, b):
    for i, item in enumerate(self.pq):
      if item[1] == b:
        elem = self.pq.pop(i)

  def refresh(self):
    heapify(self.pq)

  def add_task(self, task, priority):
    heappush(self.pq, [priority, task])
  
def generic_linkage(data):

  # ========================1==================================================
  # Initial variables (lines 2 - 4 from modern hierarchical clustering algs. )
  # ==========================================================================

  original_N = len(data) # original number of data points
  N = len(data) # the current number of data points
  sizes = np.zeros(2 * N - 1) # size of each of the cluster
  sizes[:N] = 1
  linkage = np.zeros((N-1, 4)) # standard linkage matrix 
  t_data = np.zeros((2*N-1, len(data[0])))
  t_data[:N] = data
  data = t_data
  S = set()
  for i in range(N):
    S.add(i)

  # ===========================================================================================
  # Get initial nearest neighbours list (lines 4 - 8 from modern hierarchical clustering algs.)
  # ===========================================================================================
  n_nghbr = np.zeros(2 * N-1, 'int') # initialize nearest neighbours list
  min_dist = np.ones(2 * N-1) * np.Inf # originally set the minimum distance to infinity

  # compare each point to all the points that come after
  for i, point_a in enumerate(data[:N]):

    for j, point_b in enumerate(data[i+1:N]):
      cur_dist = euclidean(point_a, point_b) # get the distance between the current points
      if cur_dist < min_dist[i]: # if it is smaller than the current minimum distance
        min_dist[i] = cur_dist # set the minimum distance to be the current distance
        n_nghbr[i] = j + i + 1

  # Create queue (line 9)
  Q = Queue()
  for i, task in enumerate(min_dist[:N]):
    Q.add_task(i, task)

  
  # MAIN LOOP
  for k in range(N-1):
    d, a = Q.remove_min()
    b = n_nghbr[a]

    # update the nearest neighbours if necessary 
    while b not in S:
      cur_point = set()
      cur_point.add(a)
      point_a = data[a]
      min_dist[a] = np.Inf
      for i in S.difference(cur_point):
        point_b = data[i] 
        cur_dist = euclidean(point_a, point_b)
        if cur_dist < min_dist[a]:
          min_dist[a] = cur_dist
          n_nghbr[a] = i 
      Q.add_task(a, min_dist[a])
      Q.refresh()
      d, a = Q.remove_min()
      b = n_nghbr[a]

    #update labels 
    S.remove(a)
    S.remove(b) 
    S.add(N + k)

    sizes[N + k] = sizes[a] + sizes[b] # set the new size
    data[N + k] = (data[a] * sizes[a] + data[b] * sizes[b]) / sizes[N + k] # get the new mean

    # Update the minimum distance and the nearest neighbours 
    cur_point = set()
    cur_point.add(N+k)
    point_a = data[N+k]
    min_dist[N + k] = np.Inf
    for i in S.difference(cur_point): 
      point_b = data[i]
      cur_dist = euclidean(point_a, point_b)
      if cur_dist < min_dist[N+k]:
        min_dist[N+k] = cur_dist
        n_nghbr[N+k] = i 

    # update the queue 
    Q.remove_elem(b)
    Q.add_task(N+k, min_dist[N+k])
    Q.refresh()

    label_a, label_b = min(a, b), max(a, b)
    linkage[k] = [label_a, label_b, d, sizes[N+k]]

  return(linkage)


def cluster(data):

  data = data.copy()
  # Initial variables
  original_N = len(data) # original number of data points
  N = len(data) # number of data points
  sizes = np.ones(N) # size of each of the clusters
  labels = np.arange(N) # labels for each cluster
  linkage = np.zeros((N-1, 4)) # standard linkage matrix


  for k in range(N-1):

    # search for the indices of the two closest points 

    min_dist = np.Inf # Current minimum distance 

    # compare each point to all the points that come after
    for i, point_a in enumerate(data[:N]):
      for j, point_b in enumerate(data[i+1:N]):
        cur_dist = euclidean(point_a, point_b) # get the distance between the current points
        if cur_dist < min_dist: # if it is smaller than the current minimum distance
          min_dist = cur_dist # set the minimum distance to be the current distance
          a = i # update the indices a, b (the indices of the points with the current minimum)
          b = j + i + 1
  
    # Get the mean of the newly formed cluster

    size_a = sizes[a] # size of the first cluster to be merged
    size_b = sizes[b] # size of the second cluster to be merged 
    new_size = size_a + size_b # size of the new cluster 
    new_mean = (data[a] * size_a + data[b] * size_b) / new_size #formula for combining means

    # update data to include the new mean 
    data[a] = new_mean # first point becomes new point
    data[b] = data[N-1] # second point becomes last point 

    # update the sizes 
    sizes[a] = new_size
    sizes[b] = sizes[N-1]
    
    # reduce N by 1 
    N = N - 1

    # output to the linkage
    label_a = labels[a]
    label_b = labels[b]
    label_a, label_b = min(label_a, label_b), max(label_a, label_b)
    

    linkage[k] = [label_a, label_b, min_dist, new_size]
  
    # update the labels 
    labels[a] = original_N + k 
    labels[b] = labels[N]
    labels[N] = -1



  return linkage

def OurMain():
  x = np.random.rand(100, 5)
  l1 = generic_linkage(x)

tau.run('OurMain()')
