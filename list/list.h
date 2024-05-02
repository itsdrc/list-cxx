#pragma once

#include<iostream>
#include<utility>

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

public:
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
			try
			{
				deepCopy(list);
			}
			catch (const std::exception& e)
			{
				clear();
				throw e;
			}
		}
		return *this;
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

	void push_back(const T& newvalue)
	{
		node* new_node = new node(newvalue, head.previous, &head);
		head.previous->next = new_node;
		head.previous = new_node;
		++nelms;
	}

	template <typename... Args>
	void emplace_back(Args &&...args)
	{
		node* new_node = new node(head.previous, &head, std::forward<Args>(args)...);
		head.previous->next = new_node;
		head.previous = new_node;
		++nelms;
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
		node* new_node = new node(newvalue, &head, head.next);
		head.next->previous = new_node;
		head.next = new_node;
		++nelms;
	}

	void push_back(T&& newvalue)
	{
		emplace_back(std::move(newvalue));
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

	std::size_t size() const noexcept
	{
		return nelms;
	}

	bool empty() const noexcept
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
		link* linker;

	public:
		friend class list;
		// std::advance setup
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		iterator() : linker(nullptr) {}
		iterator(link* linker_) : linker(linker_) {}
		iterator(const iterator& it) : linker(it.linker) {}

		iterator& operator=(const iterator& it)
		{
			if (this != &it)
				linker = it.linker;
			return *this;
		}

		iterator& operator++()
		{
			linker = linker->next;
			return *this;
		}

		iterator operator++(int)
		{
			auto aux = *this;
			linker = linker->next;
			return aux;
		}

		iterator& operator--()
		{
			linker = linker->previous;
			return *this;
		}

		iterator operator--(int)
		{
			auto aux = *this;
			linker = linker->next;
			return aux;
		}

		T& operator*()
		{
			if (dynamic_cast<node*>(linker) == nullptr)
				throw std::runtime_error("Invalid ptr to use '*' ");
			return static_cast<node*>(linker)->value;
		}

		bool operator==(iterator it) const noexcept { return linker == it.linker; }
		bool operator!=(iterator it) const noexcept { return linker != it.linker; }
	};

	iterator begin()
	{
		return head.next;
	}

	iterator end()
	{
		return &head;
	}

	iterator insert(iterator it, const T& newvalue)
	{
		link* newnode = new node(newvalue, it.linker->previous, it.linker);
		it.linker->previous->next = newnode;
		it.linker->previous = newnode;
		++nelms;
		return newnode;
	}

	template <typename... Args>
	iterator emplace(iterator it, Args &&...args)
	{
		link* newnode = new node(it.linker->previous, it.linker, std::forward<Args>(args)...);
		it.linker->previous->next = newnode;
		it.linker->previous = newnode;
		++nelms;
		return newnode;
	}

	iterator insert(iterator it, T&& newvalue)
	{
		return emplace(it, std::move(newvalue));
	}

};