#pragma once

#include<iostream>
#include<utility>

template<typename List, typename It>
struct is_valid_iterator {
	static constexpr bool iteratorConcept =
		std::same_as<It, typename List::iterator>
		|| std::same_as<It, typename List::const_iterator>
		|| std::same_as<It, typename List::reverse_iterator>
		|| std::same_as<It, typename List::const_reverse_iterator>;
};

template<class T>
class list
{
private:
	struct link
	{
		link* previous;
		link* next;
		link() : previous(nullptr), next(nullptr) {}
		link(link* prev, link* nxt) : previous(prev), next(nxt) {}

		link(const link& link)
		{
			previous = link.previous;
			next = link.next;
		}

		link& operator=(const link& link)
		{
			if (this != &link)
			{
				previous = link.previous;
				next = link.next;
			}
			return *this;
		}

		virtual ~link() {}
	};

	struct node : public link
	{
		T value;
		node(const T& value_, link* prev, link* nxt)
			: value(value_), link(prev, nxt) {}

		node() = delete;

		template<typename... Args>
		node(link* prev, link* nxt, Args&&...args)
			: link(prev, nxt), value(std::forward<Args>(args)...) {}
	};

	link head;
	std::size_t nelms;

	void deepCopy(const list& list)
	{
		link* aux = list.head.next;
		while (aux != &list.head)
		{
			node* next = dynamic_cast<node*>(aux);
			if (!next)
				throw std::runtime_error("unable to casting to node");
			push_back(next->value);
			aux = aux->next;
		}
	}

	class iteratorImpl
	{
	private:
		link* linker;

	public:
		friend class list;
		iteratorImpl(link* linker_) : linker(linker_) {}
		iteratorImpl(const iteratorImpl& itImpl) : linker(itImpl.linker) {}

		iteratorImpl& operator=(const iteratorImpl& itImpl)
		{
			if (this != &itImpl)
				linker = itImpl.linker;
			return *this;
		}

		void advance()
		{
			linker = linker->next;
		}

		void goback()
		{
			linker = linker->previous;
		}

		T& getValue()
		{
			if (dynamic_cast<node*>(linker) == nullptr)
				throw std::runtime_error("Invalid ptr to use '*' ");
			return static_cast<node*>(linker)->value;
		}

		bool operator==(const iteratorImpl& itImpl) const noexcept { return linker == itImpl.linker; }
	};

	template<class It>
	link* popPosition(It it)
	{
		if (empty())
			throw std::length_error("pop called on empty list");
		node* target = dynamic_cast<node*>(it.pimpl.get()->linker);
		if (!target)
			throw std::runtime_error("pop called on head");
		link** itlinker = &(it.pimpl.get()->linker);
		link* next = (*itlinker)->next;
		(*itlinker)->previous->next = next;
		next->previous = (*itlinker)->previous;
		delete target;
		--nelms;
		return next;
	}

	template<typename It, typename ...Args>
	link* emplaceAt(It it, Args&& ...args)
	{
		link** itlinker = &(it.pimpl.get()->linker);
		link* newnode = new node((*itlinker)->previous, (*itlinker), std::forward<Args>(args)...);
		(*itlinker)->previous->next = newnode;
		(*itlinker)->previous = newnode;
		++nelms;
		return newnode;
	}

public:

	class iterator;
	class const_iterator;

	template<class NoReverseIT>
		requires std::same_as<NoReverseIT, iterator> || std::same_as<NoReverseIT, const_iterator>
	list(NoReverseIT begin, NoReverseIT end) : list()
	{
		while (begin != end)
		{
			push_back(*begin);
			++begin;
		}
	}

	list()
	{
		head.next = &head;
		head.previous = &head;
		nelms = 0;
	}

	list(const std::initializer_list<T>& list) : list()
	{
		for (const T& e : list)
			push_back(e);
	}

	list(const list& list) : list()
	{
		deepCopy(list);
	}

	list& operator=(const list& list)
	{
		if (this != &list)
		{
			clear();
			deepCopy(list);
		}

		return *this;
	}

	bool operator==(const list& list) const noexcept
	{
		link* thislist = head.next;
		link* arglist = list.head.next;

		if (size() != list.size())
			return false;

		if (empty() && list.empty())
			return true;

		while (thislist != &head && arglist != &list.head)
		{
			if ((static_cast<node*>(thislist)->value) != (static_cast<node*>(arglist)->value))
				return false;
			thislist = thislist->next;
			arglist = arglist->next;
		}

		return true;
	}

	list(list&& list) noexcept
	{
		nelms = list.nelms;
		head.next = list.head.next;
		head.previous = list.head.previous;
		list.head.previous->next = &head;
		list.head.next->previous = &head;
		list.head.next = &list.head;
		list.head.previous = &list.head;
		list.nelms = 0;
	}

	template <typename... Args>
	void emplace_back(Args &&...args)
	{
		node* new_node = new node(head.previous, &head, std::forward<Args>(args)...);
		head.previous->next = new_node;
		head.previous = new_node;
		++nelms;
	}

	void push_back(const T& newvalue)
	{
		emplace_back(newvalue);
	}

	void push_back(T&& newvalue)
	{
		emplace_back(std::move(newvalue));
	}

	template<typename... Args>
	void emplace_front(Args&& ...args)
	{
		node* new_node = new node(&head, head.next, std::forward<Args>(args)...);
		head.next->previous = new_node;
		head.next = new_node;
		++nelms;
	}

	void push_front(const T& newvalue)
	{
		emplace_front(newvalue);
	}

	void push_front(T&& newvalue)
	{
		emplace_front(std::move(newvalue));
	}

	void pop_back()
	{
		if (empty())
			throw std::length_error("pop called on empty list");
		link* last = head.previous;
		last->previous->next = &head;
		head.previous = last->previous;
		delete last;
		--nelms;
	}

	void pop_front()
	{
		if (empty())
			throw std::length_error("pop called on empty list");
		link* front = head.next;
		head.next = front->next;
		front->next->previous = &head;
		delete front;
		--nelms;
	}

	[[nodiscard]] std::size_t size() const noexcept
	{
		return nelms;
	}

	[[nodiscard]] bool empty() const noexcept
	{
		return nelms == 0;
	}

	T& front()
	{
		if (empty())
			throw std::length_error("front called on empty list");
		return static_cast<node*>(head.next)->value;
	}

	T& back()
	{
		if (empty())
			throw std::length_error("back called on empty list");
		return static_cast<node*>(head.previous)->value;
	}

	const T& front() const
	{
		if (empty())
			throw std::length_error("front called on empty list");
		return static_cast<node*>(head.next)->value;
	}

	const T& back() const
	{
		if (empty())
			throw std::length_error("back called on empty list");
		return static_cast<node*>(head.previous)->value;
	}

	void clear()
	{
		link* aux = head.next;
		while (aux != &head)
		{
			node* target = static_cast<node*>(aux);
			aux = aux->next;
			delete target;
		}

		nelms = 0;
		head.next = &head;
		head.previous = &head;
	}

	class iterator
	{
	private:
		std::unique_ptr<iteratorImpl> pimpl;

	public:
		friend class list;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		iterator() : pimpl(nullptr) {}
		iterator(link* linker) : pimpl(std::make_unique<iteratorImpl>(linker)) {}
		iterator(const iterator& it) : pimpl(std::make_unique<iteratorImpl>(*(it.pimpl.get()))) {}

		iterator operator=(const iterator& it)
		{
			if (this != &it)
				pimpl = std::make_unique<iteratorImpl>(*(it.pimpl.get()));
			return *this;
		}

		iterator& operator++()
		{
			pimpl.get()->advance();
			return *this;
		}

		iterator operator++(int)
		{
			auto aux = *this;
			pimpl.get()->advance();
			return aux;
		}

		iterator& operator--()
		{
			pimpl.get()->goback();
			return *this;
		}

		iterator operator--(int)
		{
			auto aux = *this;
			pimpl.get()->goback();
			return aux;
		}

		T& operator*()
		{
			return pimpl.get()->getValue();
		}

		bool operator==(const iterator& it) const noexcept { return *(pimpl.get()) == *(it.pimpl.get()); }
	};

	[[nodiscard]] iterator begin() noexcept
	{
		return head.next;
	}

	[[nodiscard]] iterator end() noexcept
	{
		return &head;
	}

	class const_iterator
	{
	private:
		std::unique_ptr<iteratorImpl> pimpl;

	public:
		friend class list;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		const_iterator() : pimpl(nullptr) {}
		const_iterator(const link* linker) :pimpl(std::make_unique<iteratorImpl>(const_cast<link*>(linker))) {}
		const_iterator(const const_iterator& cit) : pimpl(std::make_unique<iteratorImpl>(*(cit.pimpl.get()))) {}
		const_iterator(const iterator& it) :pimpl(std::make_unique<iteratorImpl>(*(it.pimpl.get()))) {}

		const_iterator& operator=(const const_iterator& cit)
		{
			if (this != &cit)
				pimpl = std::make_unique<iteratorImpl>(*(cit.pimpl.get()));
			return *this;
		}

		const_iterator& operator=(const iterator& it)
		{
			if (this != &it)
				pimpl = std::make_unique<iteratorImpl>(*(it.pimpl.get()));
			return *this;
		}

		const_iterator& operator++()
		{
			pimpl.get()->advance();
			return *this;
		}

		const_iterator operator++(int)
		{
			auto aux = *this;
			pimpl.get()->advance();
			return aux;
		}

		const_iterator& operator--()
		{
			pimpl.get()->goback();
			return *this;
		}

		const_iterator operator--(int)
		{
			auto aux = *this;
			pimpl.get()->goback();
			return aux;
		}

		const T& operator*() const
		{
			return pimpl.get()->getValue();
		}

		bool operator==(const const_iterator cit) const noexcept { return *(pimpl.get()) == *(cit.pimpl.get()); }
		bool operator==(const iterator& it) const noexcept { return *(pimpl.get()) == *(it.pimpl.get()); }
	};

	[[nodiscard]] const_iterator cbegin() const noexcept
	{
		return head.next;
	}

	[[nodiscard]] const_iterator cend() const noexcept
	{
		return &head;
	}

	[[nodiscard]] const_iterator begin() const noexcept
	{
		return head.next;
	}

	[[nodiscard]] const_iterator end() const noexcept
	{
		return &head;
	}


	class reverse_iterator
	{
	private:
		std::unique_ptr<iteratorImpl> pimpl;

	public:
		friend class list;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		reverse_iterator() : pimpl(nullptr) {}
		reverse_iterator(const link* linker) :pimpl(std::make_unique<iteratorImpl>(const_cast<link*>(linker))) {}
		reverse_iterator(const iterator& it) :pimpl(std::make_unique<iteratorImpl>(*(it.pimpl.get()))) {}
		reverse_iterator(const reverse_iterator& revit) : pimpl(std::make_unique<iteratorImpl>(*(revit.pimpl.get()))) {}

		reverse_iterator& operator=(const reverse_iterator& revit)
		{
			if (this != &revit)
				pimpl = std::make_unique<iteratorImpl>(*(revit.pimpl.get()));
			return *this;
		}

		reverse_iterator& operator=(const iterator& it)
		{
			if (this != &it)
				pimpl = std::make_unique<iteratorImpl>(*(it.pimpl.get()));
			return *this;
		}

		reverse_iterator& operator++()
		{
			pimpl.get()->goback();
			return *this;
		}

		reverse_iterator operator++(int)
		{
			auto aux = *this;
			pimpl.get()->goback();
			return aux;
		}

		reverse_iterator& operator--()
		{
			pimpl.get()->advance();
			return *this;
		}

		reverse_iterator operator--(int)
		{
			auto aux = *this;
			pimpl.get()->advance();
			return aux;
		}

		T& operator*() const
		{
			return pimpl.get()->getValue();
		}

		bool operator==(const reverse_iterator revit) const noexcept { return *(pimpl.get()) == *(revit.pimpl.get()); }
		bool operator==(const iterator& it) const noexcept { return *(pimpl.get()) == *(it.pimpl.get()); }
		bool operator==(const const_iterator& cit) const noexcept { return  *(pimpl.get()) == *(cit.pimpl.get()); }
	};

	[[nodiscard]] reverse_iterator rbegin() const noexcept
	{
		return head.previous;
	}

	[[nodiscard]] reverse_iterator rend() const noexcept
	{
		return &head;
	}


	class const_reverse_iterator
	{
	private:
		std::unique_ptr<iteratorImpl> pimpl;

	public:
		friend class list;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		const_reverse_iterator() : pimpl(nullptr) {}
		const_reverse_iterator(const link* linker) :pimpl(std::make_unique<iteratorImpl>(const_cast<link*>(linker))) {}
		const_reverse_iterator(const iterator& it) :pimpl(std::make_unique<iteratorImpl>(*(it.pimpl.get()))) {}
		const_reverse_iterator(const const_iterator& cit) :pimpl(std::make_unique<iteratorImpl>(*(cit.pimpl.get()))) {}
		const_reverse_iterator(const reverse_iterator& revit) : pimpl(std::make_unique<iteratorImpl>(*(revit.pimpl.get()))) {}
		const_reverse_iterator(const const_reverse_iterator& crevit) : pimpl(std::make_unique<iteratorImpl>(*(crevit.pimpl.get()))) {}

		const_reverse_iterator& operator=(const const_reverse_iterator& crevit)
		{
			if (this != &crevit)
				pimpl = std::make_unique<iteratorImpl>(*(crevit.pimpl.get()));
			return *this;
		}

		const_reverse_iterator& operator=(const reverse_iterator& revit)
		{
			if (this != &revit)
				pimpl = std::make_unique<iteratorImpl>(*(revit.pimpl.get()));
			return *this;
		}

		const_reverse_iterator& operator=(const const_iterator& cit)
		{
			if (this != &cit)
				pimpl = std::make_unique<iteratorImpl>(*(cit.pimpl.get()));
			return *this;
		}

		const_reverse_iterator& operator=(const iterator& it)
		{
			if (this != &it)
				pimpl = std::make_unique<iteratorImpl>(*(it.pimpl.get()));
			return *this;
		}

		const_reverse_iterator& operator++()
		{
			pimpl.get()->goback();
			return *this;
		}

		const_reverse_iterator operator++(int)
		{
			auto aux = *this;
			pimpl.get()->goback();
			return aux;
		}

		const_reverse_iterator& operator--()
		{
			pimpl.get()->advance();
			return *this;
		}

		const_reverse_iterator operator--(int)
		{
			auto aux = *this;
			pimpl.get()->advance();
			return aux;
		}

		const T& operator*() const
		{
			return pimpl.get()->getValue();
		}

		bool operator==(const const_reverse_iterator& crevit) const noexcept { return *(pimpl.get()) == *(crevit.pimpl.get()); }
		bool operator==(const reverse_iterator& revit) const noexcept { return *(pimpl.get()) == *(revit.pimpl.get()); }
		bool operator==(const iterator& it) const noexcept { return *(pimpl.get()) == *(it.pimpl.get()); }
		bool operator==(const const_iterator& cit) const noexcept { return  *(pimpl.get()) == *(cit.pimpl.get()); }
	};

	[[nodiscard]] const_reverse_iterator crbegin() const noexcept
	{
		return head.previous;
	}

	[[nodiscard]] const_reverse_iterator crend() const noexcept
	{
		return &head;
	}

	template<typename It, typename ...Args>
		requires is_valid_iterator<list, It>::iteratorConcept
	It emplace(It it, Args&& ... args)
	{
		return emplaceAt<It>(it, std::forward<Args>(args)...);
	}

	template<typename It>
		requires is_valid_iterator<list, It>::iteratorConcept
	It pop(It it)
	{
		return popPosition<It>(it);
	}

	template<typename It>
		requires is_valid_iterator<list, It>::iteratorConcept
	It insert(It it, const T& newvalue)
	{
		return emplace<It>(it, newvalue);
	}

	template<typename It>
		requires is_valid_iterator<list, It>::iteratorConcept
	It insert(It it, T&& newvalue)
	{
		return emplace<It>(it, std::move(newvalue));
	}

	template<class Condition>
	std::size_t remove_if(Condition condition)
	{
		std::size_t totalRemoved = 0;
		auto it = cbegin();

		while (it != cend())
			if (condition(*it))
			{
				it = pop(it);
				++totalRemoved;
			}
			else
				++it;

		return totalRemoved;
	}
};