#ifndef STATS_H
#define STATS_H
#include <iostream>

using namespace std;

class Stats
{
  public:
      Stats() : m_cc(0), m_sp(0), m_ep(0) {};
      void cc_inc(){m_cc++;}
      void sp_inc(){m_sp++;}
      void ep_inc(){m_ep++;}
      void print_cc_stats(int iter)
      {
	  float m = m_cc;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method" << endl;
      }
      void print_sp_stats(int iter)     
      {
	  float m = m_sp;
	  float n = iter;
	  m_res = m  * 100 / n;	  
	  cout << "Player wins " << m_res << "% of times in sp method" << endl;
      }
      void print_ep_stats(int iter)    
      {
	  float m = m_ep;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in ep method" << endl;
      }
  private:
      int m_cc;
      int m_sp;
      int m_ep;
      float m_res;
};

#endif // STATS_H
