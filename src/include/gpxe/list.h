#ifndef _LIST_H
#define _LIST_H

/** @file
 *
 * Linked lists
 *
 * This linked list handling code is based on the Linux kernel's
 * list.h.
 */

#include <stddef.h>

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define LIST_POISON1 ( ( void * ) 0x00100100 )
#define LIST_POISON2 ( ( void * ) 0x00200200 )

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD_INIT( name ) { &(name), &(name) }

#define LIST_HEAD( name ) \
	struct list_head name = LIST_HEAD_INIT ( name )

#define INIT_LIST_HEAD( ptr ) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while ( 0 )

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add ( struct list_head *new,
				struct list_head *prev,
				struct list_head *next ) {
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * Add a new entry to the head of a list
 *
 * @v new	New entry to be added
 * @v head	List head to add it after
 *
 * Insert a new entry after the specified head.  This is good for
 * implementing stacks.
 */
static inline void list_add ( struct list_head *new, struct list_head *head ) {
	__list_add ( new, head, head->next );
}

/**
 * Add a new entry to the tail of a list
 *
 * @v new	New entry to be added
 * @v head	List head to add it before
 *
 * Insert a new entry before the specified head.  This is useful for
 * implementing queues.
 */
static inline void list_add_tail ( struct list_head *new,
				   struct list_head *head ) {
	__list_add ( new, head->prev, head );
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del ( struct list_head * prev,
				struct list_head * next ) {
	next->prev = prev;
	prev->next = next;
}

/**
 * Delete an entry from a list
 *
 * @v entry	Element to delete from the list
 *
 * Note that list_empty() on entry does not return true after this;
 * the entry is in an undefined state.
 */
static inline void list_del ( struct list_head *entry ) {
	__list_del ( entry->prev, entry->next );
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

/**
 * Test whether a list is empty
 *
 * @v head	List to test.
 */
static inline int list_empty ( const struct list_head *head ) {
	return head->next == head;
}

/**
 * Get the containing struct for this entry
 *
 * @v ptr	The struct list_head pointer
 * @v type	The type of the struct this is embedded in
 * @v member	The name of the list_struct within the struct
 */
#define list_entry( ptr, type, member ) \
	container_of ( ptr, type, member )

/**
 * Iterate over a list
 *
 * @v pos	The &struct list_head to use as a loop counter
 * @v head	The head for your list
 */
#define list_for_each( pos, head ) \
	for ( pos = (head)->next; pos != (head); pos = pos->next )

/**
 * Iterate over entries in a list
 *
 * @v pos	The type * to use as a loop counter
 * @v head	The head for your list
 * @v member	The name of the list_struct within the struct
 */
#define list_for_each_entry( pos, head, member )			      \
	for ( pos = list_entry ( (head)->next, typeof ( *pos ), member);      \
	      &pos->member != (head);					      \
	      pos = list_entry ( pos->member.next, typeof ( *pos ), member ) )

#endif /* _LIST_H */
