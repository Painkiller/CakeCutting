#ifndef STATS_H
#define STATS_H
#include <iostream>

using namespace std;

class Stats
{
  public:
      Stats() : m_cc(0), m_sp(0), m_ep(0) {};
      inline void cc_inc(bool success, float gap)
      {
	  if(success)
	      m_cc++;
	  m_gap_cc += gap;
      }
      inline void sp_inc(bool success, float gap)
      {
	  if(success)
	      m_sp++;
	  m_gap_sp += gap;
      }
      inline void ep_inc(bool success, float gap)
      {
	  if(success)
	      m_ep++;
	  m_gap_ep += gap;
      }

      void print_all_stats(int iter)
      {
	  print_cc_stats(iter);
	  print_sp_stats(iter);
	  print_ep_stats(iter);
      }
      void reset_stats()
      {
	  m_cc = 0;
	  m_sp = 0;
	  m_ep = 0;
	  m_gap_cc = 0;
	  m_gap_sp = 0;
	  m_gap_ep = 0;
      }
  private:
      int m_cc;
      int m_sp;
      int m_ep;
      float m_gap_cc;
      float m_gap_sp;
      float m_gap_ep;
      float m_res;
      void print_cc_stats(int iter)
      {
	  float m = m_cc;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_gap_cc << ")" << endl;
      }
      void print_sp_stats(int iter)     
      {
	  float m = m_sp;
	  float n = iter;
	  m_res = m  * 100 / n;	  
	  cout << "Player wins " << m_res << "% of times in sp method (GAP: " << m_gap_sp << ")" << endl;
      }
      void print_ep_stats(int iter)    
      {
	  float m = m_ep;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in ep method (GAP: " << m_gap_ep << ")" << endl;
      }
};

#endif // STATS_H
