#pragma once

//template<typename T>
//class SingleTon
//{
//protected:
//	SingleTon();
//	virtual ~SingleTon();
//
//public:
//	static T* GetInstance()
//	{
//		static T _instance;
//		return &_instance;
//	}
//};

#define DECLARE_SINGLE(T)			\
private:							\
	T(){}							\
	~T(){}							\
public:								\
	static T* GetInstance()			\
	{								\
		static T instance;			\
		return &instance;			\
	}								\

#define SINGLE(OBJ) OBJ::GetInstance()