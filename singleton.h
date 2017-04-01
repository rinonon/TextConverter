#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T> class Singleton
{
public:
    static T* getInstance(void)
    {
        static T _instance;
        return &_instance;
    }

protected:
    Singleton(void){};
    virtual ~Singleton(void){};

private:
    Singleton(const Singleton& single){};
    const Singleton& operator = (const Singleton& single){return *this;};
};

#endif // SINGLETON_H
