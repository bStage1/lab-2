#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <exception>
using namespace std;

class parseError
{
	string errMessage;
public:
	parseError(string er) :errMessage(er) {};
	string message()
	{
		return errMessage;
	}
};

struct sDate
{
	int day;
	int month;
	int year;
	// êîë-âî ìèíóò ñ íà÷àëà íàøåé ýðû
	unsigned long minFrom0AD()
	{
		unsigned long min = 0;
		int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		constexpr int minInDay = 60 * 24;			// êîë-âî ìèíóò â äíå
		constexpr int minInYear = 365 * minInDay;	// êîë-âî ìèíóò â îáû÷íîì ãîäó
		int cntVYear = year / 4;					// êîë-âî âèñîêîñíûõ ëåò â äàòå 
		min = (year - cntVYear)*minInYear + cntVYear*(minInYear+minInDay);	// êîë-âî ìèíóò â ãîäàõ íàøåé äàòû
		for (int i = 0; i < month-1; i++)			//	+ êîë-âî ìèíóò â ìåñÿöàõ íàøåé äàòû
			min += months[i] * minInDay;			
		min += day * minInDay;						// + êîë-âî ìèíóò â äíÿõ íàøåé äàòû
		return min;
	}
	bool check()
	{
		int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (month < 1 || month>12) return false;
		if (year % 4 == 0) months[1]++;
		if (day<1 || day > months[month - 1]) return false;
		return true;
	}
	bool inputDate()
	{
		char yn = 'y';
		do
		{
			cout << "DD: ";
			cin >> day;
			cout << "MM: ";
			cin >> month;
			cout << "YYYY: ";
			cin >> year;
			if (check()) return true;
			else
			{
				cout << "date is wrong" << endl;
				do
				{
					cout << "Would do you try again? y/n" << endl;
					cin >> yn;
				} while (!(yn == 'y' || yn == 'n'));

			}
		} while (yn == 'y');
		return false;
	}
	
};
struct sTime
{
	int hour;
	int minutes;
	bool timeCheck()
	{
		if (minutes < 0 || minutes > 59) return false;
		if (hour < 0 || hour > 23) return false;
		return true;
	}
	bool inputTime()
	{
		char yn = 'y';
		do
		{
			cout << "Hour: ";
			cin >> hour;
			cout << "Minutes: ";
			cin >> minutes;
			if (timeCheck()) return true;
			else
			{
				cout << "time is wrong" << endl;
				do
				{
					cout << "Would do you try again? y/n" << endl;
					cin >> yn;
				} while (!(yn == 'y' || yn == 'n'));

			}
		} while (yn == 'y');
		return false;
	}
};

class callRecord
{
	int m_num;
	string m_from;
	string m_to;
	//string m_date_str;
	//string m_time_str;
	sDate m_date;
	sTime m_time;
	unsigned long m_date_time;
	int m_duration;
public:
	callRecord(string& line)
	{
		istringstream iss(line);
		string tmpline;
		// ïðèíèìàåì ïîðÿäêîâûé íîìåð
		if (getline(iss, tmpline, '\t'))
		{
			m_num = stoi(tmpline); // TODO: check exception
		}
		else throw parseError("Parse error. Bad date in field 'Num' ");
		
		if (!getline(iss, m_from, '\t'))
			throw parseError("Parse error. Bad date in field 'Who' ");
		
		if (!getline(iss, m_to, '\t'))
			throw parseError("Parse error. Bad date in field 'Whom' ");

		string tmpString15;
		if (getline(iss, tmpString15, '\t'))
		{
			m_date.year = stoi(tmpString15.substr(6, 4));
			m_date.month = stoi(tmpString15.substr(3, 2));
			if (m_date.month < 1 || m_date.month>12) throw parseError("Month in date - error");
			m_date.day = stoi(tmpString15.substr(0, 2));
			int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
			if (m_date.year % 4 == 0) months[1]++;
			if (m_date.day <1 || m_date.day > months[m_date.month-1]) throw parseError("Day in date - error");
		}

		if (getline(iss, tmpString15, '\t'))
		{
			m_time.hour = stoi(tmpString15.substr(0, 2));
			m_time.minutes = stoi(tmpString15.substr(3, 2));
			if (m_time.minutes < 0 || m_time.minutes > 59) throw parseError("Minutes in time - error");
			if (m_time.hour < 0 || m_time.hour >23)throw parseError("Hours in time  - error");
		}

		m_date_time = m_date.minFrom0AD() + m_time.hour * 60 + m_time.minutes;
		
		if (getline(iss, tmpline, '\t'))
		{
			m_duration = stoi(tmpline); // TODO: check exception
		}
		else throw parseError("Parse error. Bad date in field 'Duration' ");
	}
	
	int Num() { return m_num; };
	void setNum(int num) { m_num = num; };
	string From() { return m_from; };
	string To() { return m_to; };
	unsigned long DateTime() { return m_date_time; }
	string DateStr() 
	{ 
		string strTmp = "";
		if (m_date.day < 10) strTmp += "0";
		strTmp += to_string(m_date.day) + ".";
		if (m_date.month < 10) strTmp += "0";
		strTmp += to_string(m_date.month) + "." + to_string(m_date.year);
		return strTmp;
	};
	string TimeStr() 
	{ 
		string strTmp = "";
		if (m_time.hour < 10) strTmp += "0";
		strTmp += to_string(m_time.hour) + ":";
		if (m_time.minutes < 10) strTmp += "0";
		strTmp += to_string(m_time.minutes);
		return strTmp;
	};

	int Duration() { return m_duration; };

	static callRecord* inputFromConsole()
	{
		string resultStr, strTmp;
		cout << "¹: ";
		cin >> strTmp;
		resultStr += strTmp;
		cout << "from: ";
		cin >> strTmp;
		resultStr += '\t' + strTmp;
		cout << "to: ";
		cin >> strTmp;
		resultStr += '\t' + strTmp;
		sDate d;
		d.inputDate();
		strTmp = "";
		if (d.day < 10) strTmp += "0";
		strTmp += to_string(d.day)+".";
		if (d.month < 10) strTmp += "0";
		strTmp += to_string(d.month) + "." + to_string(d.year);
		resultStr += '\t' + strTmp;
		sTime t;
		t.inputTime();
		strTmp = "";
		if (t.hour < 10) strTmp += "0";
		strTmp += to_string(t.hour) + ":";
		if (t.minutes < 10) strTmp += "0";
		strTmp += to_string(t.minutes);
		resultStr += '\t' + strTmp;
		cout << "duration: ";
		cin >> strTmp;
		resultStr += '\t' + strTmp;
		return new callRecord(resultStr);
	}
};

class callList : public list<callRecord>
{
public:
	void readFromFile(string fileName)
	{
		try
		{
			ifstream fin(fileName);
			fin.exceptions(ifstream::badbit);
			string line;
			while (getline(fin, line))
			{
				try
				{
					callRecord cr(line);
					push_back(cr);
				}
				catch (const ifstream::failure& ex)
				{
					cerr << ex.what();
				}

			}
			fin.close();
		}
		catch (const ifstream::failure& ex)
		{
			cerr << ex.what();
			exit(1);
		}
		string line;
		
		
	}
	void writeToFile(string fileName)
	{
		try
		{
			ofstream fout(fileName);
			fout.exceptions(ofstream::badbit);
			for (auto it : *this)
			{
				fout << it.Num()		<< '\t' 
					 << it.From()		<< '\t' 
					 << it.To()			<< '\t' 
					 << it.DateStr()	<< '\t'
					 << it.TimeStr()	<< '\t' 
					 << it.Duration()	<< endl;
			}
			fout.close();
		}
		catch (const ofstream::failure& ex)
		{
			cerr << ex.what();
			exit(1);
		}

	}

	void deleteLine(int num)
	{
		remove_if([num](callRecord cr) {return num == cr.Num(); }); // óäàëÿåò èç ñïèñêà ýëåìåíòû, äëÿ êîòîðûõ ëÿìáäà âîçâðàùàåò true	
	}
	void insertLine(callRecord* cr)
	{
		list<callRecord>::iterator it; // ñîçäàåì èòåðàòîð äëÿ ïîèñêà ìåñòà âñòàâêè
		for (it = this->begin(); it != this->end(); it++) // â öèêëå èùåì ìåñòî 
		{
			if (it->Num() == cr->Num()){ break; } // êàê òîëüêî íàõîäèì ìåñòî èëè ñïèñîê çàêàí÷èâàåòñÿ âûõîäèì èç öèêëà 
		}
		
		/* Àëüòåðíàòèâíûé õîä ðåøåíèÿ 1
		list<callRecord>::iterator it = this->begin();
		do
		{
			it++;
		} while (it->Num() == cr->Num() || it != this->end());*/
	
		/* Àëüòåðíàòèâíûé õîä ðåøåíèÿ 2
		for (list<callRecord>::iterator it = this->begin(); (it != this->end() || it->Num() == cr->Num()); it++); */

		this->insert(it, *cr); // âñòàâëÿåì íàøó ñòðîêó â íàéäåííîå ìåñòî
		delete cr;
	}

	void renumerate()
	{
		int i = 1;
		for (auto &cr: *this)
		{
			cr.setNum(i++);
		}
	}

	

};
