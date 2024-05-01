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

};