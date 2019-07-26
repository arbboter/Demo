#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
class CAlgorithm
{
public:
    // 冒泡排序
    // 优化版方案：当无swap时，数据已排序好
    void BubbleSort(vector<int>& v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            for (int j = i + 1; j < v.size(); j++)
            {
                if (v[i] > v[j]) swap(v[i], v[j]);
            }
        }
    }
    // 选择排序
    void SelectSort(vector<int>& v)
    {
        for (auto i=0; i<v.size(); i++)
        {
            for (int j=i+1; j<v.size(); j++)
            {
                if (v[i] > v[j]) swap(v[i], v[j]);
            }
        }
    }

    // 插入排序
    void InsertSort(vector<int>& v)
    {
        vector<int> va;
        for (auto i=0; i<v.size(); i++)
        {
            auto p = std::upper_bound(va.begin(), va.end(), v[i]);
            va.insert(p, v[i]);
        }
        v = va;
    }

    // 快速排序
    void QuickSort(vector<int>& v, int l, int h)
    {
        if (l >= h) return;
        int k = v[l];
        int i = l, j = h;
        while (i < j)
        {
#if 1
            while (i < j && v[i] <= k) i++;
            while (i < j && v[j] >= k) j--;
            if (i < j)
            {
                swap(v[i], v[j]);
                continue;
            }
            if (v[i] > k)
            {
                swap(v[l], v[i - 1]);
                i = i - 1;
            }
            else
            {
                swap(v[l], v[i + 1]);
                i = i + 1;
            }
#else
            while (i < j && v[j] >= k) j--;
            if (i < j) v[i++] = v[j];

            while (i < j && v[i] <= k) i++;
            if (i < j) v[j--] = v[i];
#endif
        }
        v[i] = k;
        QuickSort(v, l, i-1);
        QuickSort(v, i+1, h);
    }

    int BinSearch(vector<int> v, int d)
    {
        if (v.empty()) return -1;
        int l = 0, h = v.size() - 1, m = 0;
        while (l <= h)
        {
            m = l + (h - l) / 2;
            if (v[m] == d) return m;
            else if (v[m] > d) h = m - 1;
            else l = m + 1;
        }
        return -1;
    }

public:
    void test()
    {
        vector<int> v;
        int n = 10;
        for (int i=0; i<n; i++)
        {
            v.emplace_back(rand()%100);
        }

        print("原数据", v);

        vector<int> v1 = v;
        BubbleSort(v1);
        print("冒泡排序", v1);

        v1 = v;
        BubbleSort(v1);
        print("选择排序", v1);

        v1 = v;
        InsertSort(v1);
        print("插入排序", v1);

        v1 = v;
        QuickSort(v1, 0, n-1);
        print("快速排序", v1);

    }
    void print(const string& info,vector<int>& v)
    {
        cout << info << ":";
        for (auto i = 0; i < v.size(); i++)
        {
            cout << v[i] << " ";
        }
        cout << endl;
    }
};