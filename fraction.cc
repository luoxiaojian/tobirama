
#include "fraction.h"

static void _swap(int &a, int &b) {
	int tmp;
	tmp=a;
	a=b;
	b=tmp;
}

static int _gcd(int a, int b) {
	if(0==a)
		return b;
	if(0==b)
		return a;
	if(a<0)
		a=-a;
	if(b<0)
		b=-b;
	if(a>b)
		_swap(a, b);

	int c;
	for(c=a%b; c>0; c=a%b)
	{
		a=b;
		b=c;
	}
	return b;
}

static int _lcm(int a, int b) {
	assert(a>0);
	assert(b>0);
	int con=_gcd(a, b);
	con=a/con;
	con=b*con;
	return con;
}

void fraction::simplify_() {
	assert(de_!=0);
	if(de_<0)
	{
		de_=-de_;
		nu_=-nu_;
	}
	if(nu_==0) {
		de_=1;
		return;
	} else {
		int con=_gcd(nu_, de_);
		nu_=nu_/con;
		de_=de_/con;
	}
}

fraction::fraction() : nu_(0), de_(1) { }

fraction::fraction(int n) : nu_(n), de_(1) { }

fraction::fraction(int n, int d) : nu_(n), de_(d) {
	simplify_();
}

/*fraction::fraction(const fraction& parm) : nu_(parm.nu_), de_(parm.de_) {
	simplify_();
}*/

fraction& fraction::operator =(const fraction& parm) {
	nu_=parm.nu_;
	de_=parm.de_;
	simplify_();
	return *this;
}

fraction& fraction::operator+=(const fraction& parm) {
	int newDe=_lcm(de_, parm.de_);
	int t1=newDe/de_;
	int t2=newDe/parm.de_;
	int newNu=t1*nu_+t2*parm.nu_;
	de_=newDe;
	nu_=newNu;
	simplify_();
	return *this;
}

fraction& fraction::operator-=(const fraction& parm) {
	int newDe=_lcm(de_, parm.de_);
	int t1=newDe/de_;
	int t2=newDe/parm.de_;
	int newNu=t1*nu_-t2*parm.nu_;
	de_=newDe;
	nu_=newNu;
	simplify_();
	return *this;
}

float fraction::value() const{
	return ((float)nu_/(float)de_);
}

bool operator==(const fraction& lhs, const fraction& rhs) {
	return ((lhs.de_==rhs.de_)&&(lhs.nu_==rhs.nu_));
}

bool operator<(const fraction& lhs, const fraction& rhs) {
	return (lhs.value()<rhs.value());
}

bool operator>(const fraction& lhs, const fraction& rhs) {
	return (lhs.value()>rhs.value());
}

fraction operator+(const fraction& lhs, const fraction& rhs) {
	int newDe=_lcm(lhs.de_, rhs.de_);
	int t1=newDe/lhs.de_;
	int t2=newDe/rhs.de_;
	int newNu=t1*lhs.nu_+t2*rhs.nu_;
	return fraction(newNu, newDe);
}

fraction operator-(const fraction& lhs, const fraction& rhs) {
	int newDe=_lcm(lhs.de_, rhs.de_);
	int t1=newDe/lhs.de_;
	int t2=newDe/rhs.de_;
	int newNu=t1*lhs.nu_-t2*rhs.nu_;
	return fraction(newNu, newDe);
}

fraction operator*(const fraction& lhs, const fraction& rhs) {
	int newDe=lhs.de_*rhs.de_;
	int newNu=lhs.nu_*rhs.nu_;
	return fraction(newNu, newDe);
}

fraction operator/(const fraction& lhs, const fraction& rhs) {
	int newDe=lhs.de_*rhs.nu_;
	int newNu=lhs.nu_*rhs.de_;
	return fraction(newNu, newDe);
}

void fraction::output() const{
	cout<<nu_<<"/"<<de_<<endl;
}

string fraction::strval() const {
	stringstream sstr;
	if(de_==1)
		sstr<<nu_;
	else
		sstr<<nu_<<"/"<<de_;
	string res=sstr.str();
	return res;
}
