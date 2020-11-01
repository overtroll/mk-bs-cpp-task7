#include <memory>

template <typename T>
class TRefCounter {
public:
	TRefCounter()
		: ref_count(0)
	{
	}

	void Ref() {
		++ref_count;
	}

	void Deref() {
		if (--ref_count == 0) {
			delete static_cast<T*>(this);
		}
	}

	size_t RefCount() const {
		return ref_count;
	}

private:
	size_t ref_count;
};

template <typename T>
class TBasePtr {
public:
	T* operator ->() {
		return value;
	}

	const T* operator ->() const {
		return value;
	}

	T& operator *() {
		return *value
	}

	const T& operator *() const {
		return *value;
	}

	operator bool() const {
		return static_cast<bool>(value);
	}

	friend bool operator != (const TBasePtr& lhs, const TBasePtr& rhs) {
		return lhs.value != rhs.value;
	}

	friend bool operator == (const TBasePtr& lhs, const TBasePtr& rhs) {
		return lhs.value == rhs.value;
	}

protected:
	TBasePtr() = default;

	T* value = nullptr;
};

template <typename T>
class TIntrusivePtr : public TBasePtr<T> {
	using Base = TBasePtr<T>;
	using Self = TIntrusivePtr<T>;
public:
	TIntrusivePtr(T* value = nullptr) 
	{
		Reset(value);
	}

	void Reset(T* ptr = nullptr) {
		Deref();
		value = ptr;
		Ref();
	}

	void Reset(const TIntrusivePtr<T>& other) {
		Reset(other.value);
	}

	void Reset(TIntrusivePtr<T>&& other) {
		Deref();
		value = other.value;
		other.value = nullptr;
	}

	TIntrusivePtr(const TIntrusivePtr& other)
		: TIntrusivePtr(other.value)
	{
	}

	TIntrusivePtr(TIntrusivePtr&& other) 
	{
		value = other.value;
		other.value = nullptr;
	}

	TIntrusivePtr& operator = (const TIntrusivePtr& other) {
		Reset(other);
		return *this;
	}

	TIntrusivePtr& operator = (TIntrusivePtr&& other) {
		Reset(std::move(other));
		return *this;
	}

	~TIntrusivePtr() {
		Deref();
	}

private:
	void Ref() {
		if (value) {
			value->Ref();
		}
	}

	void Deref() {
		if (value) {
			value->Deref();
		}
	}
};

template <typename T, typename... Args>
TIntrusivePtr<T> MakeIntrusive(Args&&... args) {
	return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}


