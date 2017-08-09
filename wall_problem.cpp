//#define DEBUG 1

// Author: Juan M Navarro
// Email: juan.m.navarro@gmail.com
// Date: August 1st, 2017

// Instructions: 
// To compile this program: g++ wall_problem.cpp -o wall_problem
// To run this program after it has been compiled: ./wall_problem

// This program was compiled with g++ version 4.8.2 on an Ubuntu VM. The results of the "g++ -v" command are shown below

/* 
jmnavarr@ubuntu:~/dev/c$ g++ -v
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/4.8/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 4.8.2-19ubuntu1' --with-bugurl=file:///usr/share/doc/gcc-4.8/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.8 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.8 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-libmudflap --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.8-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.8-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.8-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 

*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
vector< vector<int> > wall_partitions;
vector< vector<int> > partition_permutations;
int W, H; // width and height of our wall

// check if the brick is valid (i.e. it has a width of either 2 or 3)
bool is_valid_brick(int brick)
{
    int valid_bricks[] = {2, 3};
    bool is_valid = false;    

    // iterate through valid bricks
    for(int j = 0; j < 2; j++)
    {
       // is_valid is true if the brick is in our list of valid bricks
       if(valid_bricks[j] == brick)
       {
          is_valid = true;
       }
    }

    return is_valid;
}

// check if a vector is valid (i.e. its bricks consist of bricks of width 2 or 3)
bool is_valid_vector(vector<int> v, int level)
{
    bool is_valid = true; // assume all bricks are of width 2 or 3

    for(int i = 0; i <= level; i++)
    {
       is_valid = is_valid_brick(v[i]);
       if(is_valid == false)
       {
          break;
       }
    }

    return is_valid;
}

// print vector for debugging
void print_vector(vector<int>& v, int level)
{
    for(int i = 0; i <= level; i++)
        cout << v[i] << " ";
    cout << endl;
}

// append valid wall vector to our list of wall_partitions
void append_valid_wall_vector(vector<int> v, int level)
{
    bool is_valid = is_valid_vector(v, level);

    if(is_valid == true)
    {
       vector<int> temp_vector(level + 1);
       for(int i = 0; i <= level; i++)
       {
          temp_vector[i] = v[i];
       }

       wall_partitions.push_back(temp_vector);
    }
}

// find the partitions for a number n
void partition(int n, vector<int>& v, int level){
    int first; // first is before last

    if(n < 1) return ;
    v[level] = n;
    //print_vector(v, level);
    append_valid_wall_vector(v, level);

    first = (level == 0) ? 1 : v[level-1];

    for(int i = first; i <= n/2; i++)
    {
        v[level] = i; // replace last
        partition(n-i, v, level+1);
    }
}

// printn our wall_partitions
void print_partitions()
{
    for(int i = 0; i < wall_partitions.size(); i++)
    {
       for(int j = 0; j < wall_partitions[i].size(); j++)
       {
          cout << wall_partitions[i][j] << " ";
       }

       cout << endl;
    }

    cout << "----end print partitions----" << endl;
}

// swap two numbers
void swap(int *x1, int *x2)
{
    int x = *x1;
    *x1 = *x2;
    *x2 = x;
}

// find all the permutations for vector v
void permutation(vector<int> v, int st, int ls)
{
    int i = 0;
    if(st == ls)
    {
        int k;
        vector<int> temp_vector(ls);

        for(k = 0; k < ls; k++)
        {
            //cout << v[k] << " ";
            temp_vector[k] = v[k];
        }

        partition_permutations.push_back(temp_vector);

        //cout << endl;
    }
    else
    {
        for(i = st; i < ls; i++)
        {
            swap(v[st], v[i]);
            permutation(v, st + 1, ls);
            swap(v[st], v[i]);
        }
    }
}

// print a matrix to STDOUT
void print_matrix(vector< vector<int> > matrix)
{
   for(int i = 0; i < matrix.size(); i++)
   {
       for(int j = 0; j < matrix[i].size(); j++)
       {
          cout << matrix[i][j] << " ";
       }

       cout << endl;
   }

   cout << "----end print matrix----" << endl;
}

// get unique permutations
void get_permutations()
{
   for(int i = 0; i < wall_partitions.size(); i++)
   {
      permutation(wall_partitions[i], 0, wall_partitions[i].size());
   }

   sort(partition_permutations.begin(), partition_permutations.end()); // sort and get unique only
   partition_permutations.erase(unique(partition_permutations.begin(), partition_permutations.end()), partition_permutations.end());
}

// check if int x is in vector v
bool is_in_array(vector<int> v, int x)
{
   bool result = false;

   for(int i = 0; i < v.size(); i++)
   {
      if(v[i] == x)
      {
         result = true;
         break;
      }
   }

   return result;
}

// check if two vectors can be stacked (i.e. without creating a crack)
int can_row_stack_on_row(vector<int> v1, vector<int> v2)
{
   int result = 1;
   int v1_sum = 0, v2_sum = 0;   
   vector<int> v1_sum_vector;
   vector<int> v2_sum_vector;
   int i = 0, j = 0;
   bool in_array = false;

   for(i = 0; i < v1.size(); i++)
   {
      v1_sum_vector.push_back(v1_sum + v1[i]);
      v1_sum += v1[i];
   }

   for(i = 0; i < v2.size(); i++)
   {
      v2_sum_vector.push_back(v2_sum + v2[i]);
      v2_sum += v2[i];
   }

   for(i = 0; i < v1_sum_vector.size(); i++)
   {
      in_array = is_in_array(v2_sum_vector, v1_sum_vector[i]);
      if(in_array && v1_sum_vector[i] < W) // we have found a crack
      {
         result = 0;
      }
   }   

   return result;
}

// function to multiply a matrix NxN by a vector of size N
vector<int> multiply_matrix_with_vector(vector< vector<int> > m, vector<int> v)
{
   vector<int> result;
   int val = 0;

   for(int i = 0; i < m.size(); i++)
   {
      val = 0;

      for(int j = 0; j < m[i].size(); j++)
      {
         val += m[i][j] * v[j];
      }

      result.push_back(val);
   } 

   return result;
}

// get our lucky number
int calculate_crack_free_count()
{
   long long count = 0;
   int i = 0, j = 0;
   int row_stack_count = 0;
   vector< vector<int> > vector_compatibility_matrix;
   vector<int> v_multiply;
   int unique_vector_count = partition_permutations.size();

   v_multiply.assign(unique_vector_count, 1); // create identity vector

   for(i = 0; i < unique_vector_count; i++)
   {
      vector<int> temp_vector;
      temp_vector.assign(unique_vector_count, 0);
      vector_compatibility_matrix.push_back(temp_vector); // create NxN matrix filled with all 0's initially
   }   

   for(i = 0; i < unique_vector_count; i++)
   {
      for(j = 0; j < unique_vector_count; j++)
      {
         vector_compatibility_matrix[i][j] = can_row_stack_on_row(partition_permutations[i], partition_permutations[j]);
         row_stack_count += vector_compatibility_matrix[i][j];
      } 
   }

#ifdef DEBUG
   cout << "Row stack count: " << row_stack_count << endl;

   print_matrix(vector_compatibility_matrix);
#endif

   for(i = 0; i < H - 1; i++) // multiply the vector H - 1 times
   {
      v_multiply = multiply_matrix_with_vector(vector_compatibility_matrix, v_multiply);
   }

   for(i = 0; i < unique_vector_count; i++)
   {
      count += v_multiply[i];
   }

   return count;
}

int main()
{
   long long count;
#ifdef DEBUG
   cout << "input width:";
   cin >> W;

   cout << "input height:";
   cin >> H;
#else
   W = 32;
   H = 10;
#endif
   vector<int> v(W);

   partition(W, v, 0);

#ifdef DEBUG
   print_partitions();
#endif

   get_permutations();

#ifdef DEBUG
   print_matrix(partition_permutations);
#endif

   count = calculate_crack_free_count();

   cout << count << endl; 
}
