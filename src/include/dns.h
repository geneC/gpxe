#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H

#include "stdint.h"
#include "in.h"
#include "ip.h"
#include "udp.h"

/*
 * Constants
 *
 */

#define DNS_TYPE_A		1
#define DNS_TYPE_CNAME		5
#define DNS_TYPE_ANY		255

#define DNS_CLASS_IN		1
#define DNS_CLASS_CS		2
#define DNS_CLASS_CH		3
#define DNS_CLASS_HS		4

#define DNS_FLAG_QUERY		( 0x00 << 15 )
#define DNS_FLAG_RESPONSE	( 0x01 << 15 )
#define DNS_FLAG_QR(flags)	( (flags) & ( 0x01 << 15 ) )
#define DNS_FLAG_OPCODE_QUERY	( 0x00 << 11 )
#define DNS_FLAG_OPCODE_IQUERY	( 0x01 << 11 )
#define DNS_FLAG_OPCODE_STATUS	( 0x02 << 11 )
#define DNS_FLAG_OPCODE(flags)	( (flags) & ( 0x0f << 11 ) )
#define DNS_FLAG_RD		( 0x01 << 8 )
#define DNS_FLAG_RA		( 0x01 << 7 )
#define DNS_FLAG_RCODE_OK	( 0x00 << 0 )
#define DNS_FLAG_RCODE_NX	( 0x03 << 0 )
#define DNS_FLAG_RCODE(flags)	( (flags) & ( 0x0f << 0 ) )

#define	DNS_UDP_PORT		53
#define	DNS_MAX_RETRIES		3
#define	DNS_MAX_CNAME_RECURSION	0x30

/*
 * DNS protocol structures
 *
 */
struct dns_header {
	uint16_t	id;
	uint16_t	flags;
	uint16_t	qdcount;
	uint16_t	ancount;
	uint16_t	nscount;
	uint16_t	arcount;
} __attribute__ (( packed ));

struct dns_query_info {
	uint16_t	qtype;
	uint16_t	qclass;
} __attribute__ (( packed ));

struct dns_query {
	struct iphdr	ip;
	struct udphdr	udp;
	struct dns_header dns;
	char		payload[ 256 + sizeof ( struct dns_query_info ) ];
} __attribute__ (( packed ));

struct dns_rr_info {
	uint16_t	type;
	uint16_t	class;
	uint32_t	ttl;
	uint16_t	rdlength;
} __attribute__ (( packed ));

struct dns_rr_info_a {
	struct dns_rr_info info;
	struct in_addr in_addr;
} __attribute__ (( packed ));

struct dns_rr_info_cname {
	struct dns_rr_info info;
	char		cname[0];
} __attribute__ (( packed ));

/*
 * Functions in dns.c (used by nmb.c)
 *
 */
extern struct dns_header * dns_query ( struct dns_query *query,
				       unsigned int query_len, 
				       struct sockaddr_in *nameserver );
extern struct dns_rr_info * dns_find_rr ( struct dns_query *query,
					  struct dns_header *reply );

#endif /* DNS_RESOLVER_H */
