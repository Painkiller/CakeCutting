#ifndef STATS_H
#define STATS_H
#include <iostream>
#include "shared.h"

using namespace std;

class Stats
{
  public:
      Stats() : m_cc_map(0), m_sp_map(0), m_ep_map(0), 
		m_cc_piece(0), m_sp_piece(0), m_ep_piece(0),
		m_cc_player(0), m_sp_player(0), m_ep_player(0),
		m_fake_result_cc(0), m_fake_result_sp(0),m_fake_result_ep(0),m_iter(0){};
      inline void inc(bool success, float gap, StatType stat, MethodType method)
      {
	  switch(method)
	  {
	      case CC_METHOD:
	      {
		  switch(stat)
		  {
		      case MAP_STAT:
		      {
			  if(success)
			      m_cc_map++;
			  m_map_gap_cc += gap;
		      }
		      break;
		      case PIECE_STAT:
		      {
			  if(success)
			      m_cc_piece++;
			  m_piece_gap_cc += gap;
		      }
		      break;
		      case PLAYER_STAT:
		      {
			  if(success)
			      m_cc_player++;
			  m_player_gap_cc += gap;
		      }
		      break;
		      case COMPARE_STAT:
		      {
			  if(success)
			      m_cc_compare++;
			  m_compare_gap_cc += gap;
		      }
		      break;
		  }
	      }
	      break;
	      case SP_METHOD:
	      {
		  switch(stat)
		  {
		      case MAP_STAT:
		      {
			  if(success)
			      m_sp_map++;
			  m_map_gap_sp += gap;
		      }
		      break;
		      case PIECE_STAT:
		      {
			  if(success)
			      m_sp_piece++;
			  m_piece_gap_sp += gap;
		      }
		      break;
			case PLAYER_STAT:
		      {
			  if(success)
			      m_sp_player++;
			  m_player_gap_sp += gap;
		      }
		      break;
		      case COMPARE_STAT:
		      {
			  if(success)
			      m_sp_compare++;
			  m_compare_gap_sp += gap;
		      }
		      break;
		  }
	      }
	      break;
	      case EP_METHOD:
	      {
		  switch(stat)
		  {
		      case MAP_STAT:
		      {
			  if(success)
			      m_ep_map++;
			  m_map_gap_ep += gap;
		      }
		      break;
		      case PIECE_STAT:
		      {
			  if(success)
			      m_ep_piece++;
			  m_piece_gap_ep += gap;
		      }
		      break;
		      case PLAYER_STAT:
		      {
			  if(success)
			      m_ep_player++;
			  m_player_gap_ep += gap;
		      }
		      break;
		      case COMPARE_STAT:
		      {
			  if(success)
			      m_ep_compare++;
			  m_compare_gap_ep += gap;
		      }
		      break;
		  }
	      }
	      break;
	  }
      }

      void print_all_stats(int iter)
      {
	  print_cc_stats(iter);
	  print_sp_stats(iter);
	  print_ep_stats(iter);
      }
      void reset_stats()
      {
	  m_fake_result_cc = 0;
	  m_fake_result_sp = 0;
	  m_fake_result_ep = 0;
	  m_cc_map = 0;
	  m_sp_map = 0;
	  m_ep_map = 0;
	  m_map_gap_cc = 0;
	  m_map_gap_sp = 0;
	  m_map_gap_ep = 0;
	  m_cc_piece = 0;
	  m_sp_piece = 0;
	  m_ep_piece = 0;
	  m_piece_gap_cc = 0;
	  m_piece_gap_sp = 0;
	  m_piece_gap_ep = 0;
	  m_cc_player = 0;
	  m_sp_player = 0;
	  m_ep_player = 0;
	  m_player_gap_cc = 0;
	  m_player_gap_sp = 0;
	  m_player_gap_ep = 0;
	  m_iter = 0;
	  m_cc_compare = 0;
	  m_sp_compare = 0;
	  m_ep_compare = 0;
	  m_compare_gap_cc = 0;
	  m_compare_gap_sp = 0 ;
	  m_compare_gap_ep = 0 ;
      }
      void store(float fake_result, MethodType method)
      {
	  switch(method)
	  {
	    case CC_METHOD:
		m_fake_result_cc = fake_result;
	    break;
	    case SP_METHOD:
		 m_fake_result_sp = fake_result;
	    break;
	    case EP_METHOD:
		 m_fake_result_ep = fake_result;
	    break;
	  }
      }
      float load(MethodType method)
      {	  
	  switch(method)
	  {
	    case CC_METHOD:
		return m_fake_result_cc;
	    break;
	    case SP_METHOD:
		return m_fake_result_sp;
	    break;
	    case EP_METHOD:
		return m_fake_result_ep;
	    break;
	  }
      }
      float get_gap(MethodType method)
      {
	  switch(method)
	  {
	    case CC_METHOD:
		return m_compare_gap_cc;
	    break;
	    case SP_METHOD:
		return m_compare_gap_sp ;
	    break;
	    case EP_METHOD:
		return m_compare_gap_ep ;
	    break;
	  }
      }
      float get_perc(MethodType method)
      {
	  float perc;
	  switch(method)
	  {
	    case CC_METHOD:
		m_iter++;
		perc = (float)m_cc_compare * 100 / (float)m_iter;
	    break;
	    case SP_METHOD:
		perc = (float)m_sp_compare * 100 / (float)m_iter;
	    break;
	    case EP_METHOD:
		perc = (float)m_ep_compare * 100 / (float)m_iter;
	    break;
	  }
	  return perc;
      }
  private:
      int m_cc_map, m_sp_map, m_ep_map;
      int m_cc_piece, m_sp_piece, m_ep_piece;
      int m_cc_player, m_sp_player, m_ep_player;
      int m_cc_compare, m_sp_compare, m_ep_compare;
      float m_map_gap_cc, m_map_gap_sp, m_map_gap_ep;
      float m_piece_gap_cc, m_piece_gap_sp, m_piece_gap_ep;
      float m_player_gap_cc, m_player_gap_sp, m_player_gap_ep;
      float m_compare_gap_cc, m_compare_gap_sp, m_compare_gap_ep;
      float m_res;
      float m_fake_result_cc, m_fake_result_sp, m_fake_result_ep;
      int m_iter;
     
      void print_cc_stats(int iter)
      {
	  cout << "--------- C&C METHOD ---------" << endl;
	  cout << endl;
	  float m = m_cc_map;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "TRUEMAP vs. FAKEMAP"<<endl;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_map_gap_cc << ")" << endl;
	  m = m_cc_piece;
	  m_res = m  * 100 / n;
	  cout << "PIECE OWNED vs. OTHER PIECES"<<endl;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_piece_gap_cc << ")" << endl;
	  m = m_cc_player;
	  cout << "PLAYER EVALUATION vs. OTHER PLAYER EVALUATION"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_player_gap_cc << ")" << endl;
	  m = m_cc_compare;
	  cout << "FAKE GAME vs. REAL GAME"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_compare_gap_cc << ")" << endl;
	  cout << endl;
      }
      void print_sp_stats(int iter)     
      {
	  cout << "--------- SP METHOD ---------" << endl;
	  cout << endl;
	  float m = m_sp_map;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "TRUEMAP vs. FAKEMAP"<<endl;
	  cout << "Player wins " << m_res << "% of times in sp method (GAP: " << m_map_gap_sp << ")" << endl;
	  m = m_sp_piece;
	  m_res = m  * 100 / n;
	  cout << "PIECE OWNED vs. OTHER PIECES"<<endl;
	  cout << "Player wins " << m_res << "% of times in sp method (GAP: " << m_piece_gap_sp << ")" << endl;
	  m = m_sp_player;
	  cout << "PLAYER EVALUATION vs. OTHER PLAYER EVALUATION"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in sp method (GAP: " << m_player_gap_sp << ")" << endl;
	  m = m_sp_compare;
	  cout << "FAKE GAME vs. REAL GAME"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_compare_gap_sp << ")" << endl;
	  cout << endl;
      }
      void print_ep_stats(int iter)    
      {
	  cout << "--------- EP METHOD ---------" << endl;
	  cout << endl;
	  float m = m_ep_map;
	  float n = iter;
	  m_res = m  * 100 / n;
	  cout << "TRUEMAP vs. FAKEMAP"<<endl;
	  cout << "Player wins " << m_res << "% of times in ep method (GAP: " << m_map_gap_ep << ")" << endl;
	  m = m_ep_piece;
	  m_res = m  * 100 / n;
	  cout << "PIECE OWNED vs. OTHER PIECES"<<endl;
	  cout << "Player wins " << m_res << "% of times in ep method (GAP: " << m_piece_gap_ep << ")" << endl;
	  m = m_ep_player;
	  cout << "PLAYER EVALUATION vs. OTHER PLAYER EVALUATION"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in ep method (GAP: " << m_player_gap_ep << ")" << endl;
	  m = m_ep_compare;
	  cout << "FAKE GAME vs. REAL GAME"<<endl;
	  m_res = m  * 100 / n;
	  cout << "Player wins " << m_res << "% of times in c&c method (GAP: " << m_compare_gap_ep << ")" << endl;
	  cout << endl;
      }
};

#endif // STATS_H
