/*
CSE201 - Object Oriented Programming C++
Tutorial 3 - A new tutorial on dynamic memory allocation and git
Yubo Cai
Date: 2022-07-02
*/

#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h> // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

// Exercise 1 - Extend an array
double *extend_array(double *array, int length, int new_size)
{
  double *new_array = new double[new_size];
  for (int i = 0; i < length; i++)
  {
    new_array[i] = array[i];
  }

  delete[] array;
  return new_array;
}

// Exercise 2 - Shrink an array
double *shrink_array(double *array, int length, int new_size)
{
  double *new_array = new double[new_size];
  for (int i = 0; i < new_size; i++)
  {
    new_array[i] = array[i];
  }

  delete[] array;
  return new_array;
}

// Exercise 3 - Append new elements to an array
double *append_to_array(double element,
                        double *array,
                        int &current_size,
                        int &max_size)
{
  if (current_size >= max_size)
  {
    array = extend_array(array, current_size, max_size + 5);
    max_size += 5;
  }

  array[current_size] = element;
  current_size += 1;

  return array;
}

// Exercise 4 - Append new elements to an array
double *remove_from_array(double *array,
                          int &current_size,
                          int &max_size)
{
  if (current_size > 0)
  {
    current_size -= 1;
    if (current_size <= max_size - 5)
    {
      array = shrink_array(array, current_size, max_size - 5);
      max_size -= 5;
    }
  }

  return array;
}

// Exercise 5 - Log the telemetry of a projectile
bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double *&telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size)
{
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (!hit_target) && (!hit_obstacle))
  {
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);

    double *target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL)
    {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    }
    else if (find_collision(x, y, obstacles, tot_obstacles) != NULL)
    {
      hit_obstacle = true;
    }
    else
    {
      t = t + simulation_interval;
      y = v0_y * t - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

// Exercise 6 - Merging multiple telemetries
void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double *&global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size)
{
  // 先把所有的telemetry合并到一个数组中，telemetries中的数据是每三个为一组，分别是t, x, y，然后再按照index3来进行排序
  for (int i = 0; i < tot_telemetries; i++)
  {
    for (int j = 0; j < telemetries_sizes[i]; j++)
    {
      global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
    }
  }

  // 对数组进行排序, 用冒泡排序
  int index = global_telemetry_current_size / 3;

  for (int i = 0; i < index; i++)
  {
    for (int j = i + 1; j < index; j++)
    {
      if (global_telemetry[i * 3] > global_telemetry[j * 3])
      {
        double temp = global_telemetry[i * 3];
        global_telemetry[i * 3] = global_telemetry[j * 3];
        global_telemetry[j * 3] = temp;

        temp = global_telemetry[i * 3 + 1];
        global_telemetry[i * 3 + 1] = global_telemetry[j * 3 + 1];
        global_telemetry[j * 3 + 1] = temp;

        temp = global_telemetry[i * 3 + 2];
        global_telemetry[i * 3 + 2] = global_telemetry[j * 3 + 2];
        global_telemetry[j * 3 + 2] = temp;
      }
    }
  }
}
