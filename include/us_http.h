#ifndef US_HTTP_H
#define US_HTTP_H

/*
Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of J.D. Koftinoff Software, Ltd.. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL J.D. KOFTINOFF SOFTWARE, LTD.. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "us_world.h"

#include "us_allocator.h"
#include "us_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct us_http_header_item_s
{
    const char *m_key;
    const char *m_value;
    struct us_http_header_item_s *m_next;
} us_http_header_item_t;

typedef struct us_http_header_item_list_s
{
    void ( *destroy )( struct us_http_header_item_list_s *self );

    us_http_header_item_t *( *add )( struct us_http_header_item_list_s *self, const char *key, const char *value );

    us_http_header_item_t
        *( *addn )( struct us_http_header_item_list_s *self, const char *key, int key_len, const char *value, int value_len );

    bool ( *remove )( struct us_http_header_item_list_s *self, us_http_header_item_t *item );

    us_http_header_item_t *( *find )( struct us_http_header_item_list_s *self, const char *key );

    us_allocator_t *m_allocator;
    us_http_header_item_t *m_first;

} us_http_header_item_list_t;

us_http_header_item_list_t *us_http_header_item_list_create( us_allocator_t *allocator );

void us_http_header_item_list_destroy( us_http_header_item_list_t *self );

us_http_header_item_t *us_http_header_item_list_add( us_http_header_item_list_t *self, const char *key, const char *value );

us_http_header_item_t *us_http_header_item_list_addn(
    us_http_header_item_list_t *self, const char *key, int key_len, const char *value, int value_len );

bool us_http_header_item_list_remove( us_http_header_item_list_t *self, us_http_header_item_t *item );

us_http_header_item_t *us_http_header_item_list_find( us_http_header_item_list_t *self, const char *key );

typedef struct us_http_request_header_s
{
    void ( *destroy )( struct us_http_request_header_s * );

    us_allocator_t *m_allocator;

    const char *m_method;
    const char *m_path;
    const char *m_version;

    us_http_header_item_list_t *m_items;

} us_http_request_header_t;

us_http_request_header_t *us_http_request_header_create( us_allocator_t *allocator );

void us_http_request_header_destroy( us_http_request_header_t *self );

bool us_http_request_header_set_method( us_http_request_header_t *self, const char *method );
bool us_http_request_header_set_methodn( us_http_request_header_t *self, const char *method, int len );

bool us_http_request_header_set_version( us_http_request_header_t *self, const char *version );
bool us_http_request_header_set_versionn( us_http_request_header_t *self, const char *version, int len );

bool us_http_request_header_set_path( us_http_request_header_t *self, const char *path );

bool us_http_request_header_set_pathn( us_http_request_header_t *self, const char *path, int len );

bool us_http_request_header_set_content_type( us_http_request_header_t *self, const char *content_mime_type );

bool us_http_request_header_set_content_length( us_http_request_header_t *self, int32_t content_length );

bool us_http_request_header_set_connection_close( us_http_request_header_t *self );

int32_t us_http_request_header_get_content_length( const us_http_request_header_t *self, int32_t default_length );

const char *us_http_request_header_get_content_type( const us_http_request_header_t *self );

bool us_http_request_header_init_helper( us_http_request_header_t *self,
                                         const char *method,
                                         const char *host,
                                         const char *path,
                                         const char *content_type,
                                         int32_t content_length );

bool us_http_request_header_init_get( us_http_request_header_t *self, const char *host, const char *path );

bool us_http_request_header_init_delete( us_http_request_header_t *self, const char *host, const char *path );

bool us_http_request_header_init_post(
    us_http_request_header_t *self, const char *host, const char *path, const char *content_type, uint32_t content_length );

bool us_http_request_header_init_put(
    us_http_request_header_t *self, const char *host, const char *path, const char *content_type, uint32_t content_length );

typedef struct us_http_response_header_s
{
    void ( *destroy )( struct us_http_response_header_s * );

    us_allocator_t *m_allocator;
    int m_code;
    const char *m_version;
    us_http_header_item_list_t *m_items;
} us_http_response_header_t;

us_http_response_header_t *us_http_response_header_create( us_allocator_t *allocator );

void us_http_response_header_destroy( us_http_response_header_t *self );

bool us_http_response_header_set_version( us_http_response_header_t *self, const char *version );

bool us_http_response_header_set_versionn( us_http_response_header_t *self, const char *version, int len );

bool us_http_response_header_set_content_length( us_http_response_header_t *self, ssize_t content_length );

bool us_http_response_header_set_connection_close( us_http_response_header_t *self );

bool us_http_response_header_set_content_type( us_http_response_header_t *self, const char *content_mime_type );

bool us_http_response_header_set_no_cache( us_http_response_header_t *self );

int32_t us_http_response_header_get_content_length( const us_http_response_header_t *self, int32_t default_length );

const char *us_http_response_header_get_content_type( const us_http_response_header_t *self );

bool us_http_response_header_init_error( us_http_response_header_t *self,
                                         int32_t http_error_code,
                                         const char *content_type,
                                         uint32_t content_length );

bool us_http_response_header_init_redirect( us_http_response_header_t *self,
                                            int32_t http_redirect_code,
                                            const char *redirect_to_url );

bool us_http_response_header_init_ok( us_http_response_header_t *self,
                                      int32_t http_ok_code,
                                      const char *content_type,
                                      ssize_t content_length,
                                      bool connection_close );

bool us_http_response_header_create_ok( us_http_response_header_t *self,
                                        int32_t http_ok_code,
                                        const char *content_type,
                                        uint32_t content_length );

bool us_http_response_header_flatten( us_http_response_header_t *self, us_buffer_t *buf );

bool us_http_request_header_flatten( us_http_request_header_t *self, us_buffer_t *buf );

bool us_http_header_item_list_flatten( us_http_header_item_list_t *self, us_buffer_t *buf );

bool us_http_response_header_parse( us_http_response_header_t *self, us_buffer_t *buf );

bool us_http_request_header_parse( us_http_request_header_t *self, us_buffer_t *buf );

bool us_http_header_item_list_parse( us_http_header_item_list_t *self, us_buffer_t *buf );

const char *us_http_reason_phrase( int code );

#ifdef __cplusplus
}
#endif

#endif
