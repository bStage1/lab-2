#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "callRecord.h"
#include <exception>



using namespace std;

ifstream fin;
const char fileName[] = "Table.txt";


class cr_filter
{
	string m_from;
	string m_to;
	sDate m_since;
	sDate m_before;
	void checkDate(tm dt)
	{
		int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (dt.tm_mon < 1 || dt.tm_mon>12) throw parseError("Month in date - error");
		if (dt.tm_year % 4 == 0) months[1]++;
		if (dt.tm_mday<1 || dt.tm_mday > months[dt.tm_mon - 1]) throw parseError("Day in date - error");
	}
public:
	cr_filter() : m_from(""), m_to(""), m_since{ 0,0,0 }, m_before{0,0,0} {}; 
	void setFrom(string from) { m_from = from; }; 
	string From() { return m_from; };
	void setTo(string to) { m_to = to; };
	string To() { return m_to; };

	void setSince(sDate& dt) { m_since = dt; }
	unsigned long Since() { return m_since.minFrom0AD(); };
	void setBefore(sDate& dt) {	m_before = dt; }
	unsigned long Before() { return m_before.minFrom0AD(); };
	
};

void output(cr_filter& flt)
{
	fin.exceptions(ifstream::badbit);
	try
	{
		fin.open(fileName);
		
		cout << setw(5) << "N"
			<< setw(10) << "from"
			<< setw(10) << "to"
			<< setw(20) << "date"
			<< setw(10) << "time"
			<< setw(10) << "duration"
			<< endl;

		string line;
		//callRecord* cr;
		while (getline(fin, line))
		{
			// new callRecord(line);
			callRecord cr(line);
			
			if ((flt.From() == "" || flt.From() == cr.From()) &&
				(flt.To() == "" || flt.To() == cr.To()) &&
				(flt.Since() == 0 || flt.Since() <= cr.DateTime()) &&
				(flt.Before() == 0 || flt.Before() > cr.DateTime()))
			{
				cout << setw(5) << cr.Num()
					<< setw(10) << cr.From()
					<< setw(10) << cr.To()
					<< setw(20) << cr.DateStr()
					<< setw(10) << cr.TimeStr()
					<< setw(10) << cr.Duration()
					<< endl;
			}
		}
		fin.close();
	}
	catch (const ifstream::failure& ex)
	{
		cerr << ex.what();
		cout << "îøèáêà îòêðûòèÿ ôàéëà" << endl;
	}

	

	
}



int menu()
{
	setlocale(LC_ALL, "rus");
	cout << "ìåíþ" << endl;
	cout << "1: âûâîä ïîëíîãî ñïèñêà çâîíêîâ" << endl;
	cout << "2: äîáàâèòü ïî íîìåðó" << endl;
	cout << "3: óäàëèòü ïî íîìåðó" << endl;
	cout << "4: ïåðåíóìåðîâàòü ñïèñîê" << endl;
	cout << "5: âûâîä ñ äàòû ïî äàòó" << endl;
	cout << "6: ñîðòèðîâêà ïî äàâíîñòè" << endl;
	cout << "7: ñîðòèðîâêà ïî äëèòåëüíîñòè" << endl;
	cout << "8: âûâîä îò êîíêðåòíîãî ÷åëîâåêà" << endl;
	cout << "9: âûâîä ê êîíêðåòíîìó ÷åëîâåêó" << endl;
	cout << "q: âûéòè èç ïðîãðàììû" << endl;
	char ch;
	cin >> ch;
	
	sDate dt_s, dt_b;
	string str;

	switch (ch)
	{
	case('1'): // âûâîä ïîëíîãî ñïèñêà çâîíêîâ
	{
		cr_filter fltr;
		output(fltr);
		break;
	}
	case('2'): // äîáàâèòü ïî íîìåðó
	{
		callList cl;					// ñîçäàåì ñïèñîê
		cl.readFromFile(fileName);	// ÷èòàåì äàííûå èç ôàéëà
		cl.insertLine(callRecord::inputFromConsole());
		cl.writeToFile(fileName);
	}
	break;
	case('3'): // óäàëèòü ïî íîìåðó
	{
		int n;
		cout << "what number to delete?" << endl;
		cin >> n;
		callList cl;
		cl.readFromFile(fileName);
		cl.deleteLine(n);
		cl.writeToFile(fileName);
		break;
	}
	case('4'): // ïåðåíóìåðîâàòü
	{
		callList cl;
		cl.readFromFile(fileName);
		cl.renumerate(); // åñëè íîìåð äîëæåí áûòü óíèêàëüíûì, ïåðåíóìåðîâûâàåì ñïèñîê
		cl.writeToFile(fileName);
		break;
	}
	case('5'): // âûâîä ñ äàòû ïî äàòó
	{
		cr_filter fltr;
		cout << "since? (DD:MM:YYYY)" << endl;
		if (!dt_s.inputDate()) break;

		fltr.setSince(dt_s);
		cout << "before? (DD:MM:YYYY)" << endl;
		if (!dt_b.inputDate()) break;
		fltr.setBefore(dt_b);
		output(fltr);
		break;
	}
		
	case('6'): // ñîðòèðîâêà ïî äàâíîñòè
	{
		callList cl;
		cl.readFromFile(fileName);
		cl.sort([](callRecord& cr1, callRecord& cr2) {return (cr1.DateTime()) < (cr2.DateTime());  });
		cl.writeToFile(fileName);
	}
		break;
	case('7'): // ñîðòèðîâêà ïî äëèòåëüíîñòè
	{
		callList cl;
		cl.readFromFile(fileName);
		cl.sort([](callRecord& cr1, callRecord& cr2) {return cr1.Duration() < cr2.Duration(); });
		cl.writeToFile(fileName);
	}
		break;
	case('8'): // âûâîä îò êîíêðåòíîãî ÷åëîâåêà
	{
		cr_filter fltr;
		cout << "who calling?" << endl;
		cin >> str;
		fltr.setFrom(str);
		output(fltr);
		break;
	}		
	case('9'): // âûâîä ê êîíêðåòíîìó ÷åëîâåêó
	{
		cr_filter fltr;
		cout << "who calling?" << endl;
		cin >> str;
		fltr.setTo(str);
		output(fltr);
		break;
	}		
	case('q'):
		return 0;
		break;
	default:
		break;
	}
	return 1;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	while (menu());
	

}
