/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_geo.h"
#include "math.h"

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_geo_get_distance, 0, 0, 4)
	ZEND_ARG_INFO(0, lat1)
	ZEND_ARG_INFO(0, lng1)
	ZEND_ARG_INFO(0, lat2)
        ZEND_ARG_INFO(0, lng2)
ZEND_END_ARG_INFO()

/* If you declare any globals in php_geo.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(geo)
*/

/* True global resources - no need for thread safety here */
static int le_geo;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("geo.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_geo_globals, geo_globals)
    STD_PHP_INI_ENTRY("geo.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_geo_globals, geo_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_geo_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_geo_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "geo", arg);

	RETURN_STR(strg);
}
PHP_FUNCTION(geo_get_distance)
{
    double lat1, lng1, lat2, lng2;
    int lat1_len, lng1_len, lat2_len, lng2_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &lat1, &lng1, &lat2, &lng2) == FAILURE) {
        return;
    }
    int earthRadius = 6378138;
    double pi = 3.14159265358;
    lat1 = (lat1 * pi) / 180;
    lng1 = (lng1 * pi) / 180;
    lat2 = (lat2 * pi) / 180;
    lng2 = (lng2 * pi) / 180;
    double calcLongitude = lng2 - lng1;
    double calcLatitude = lat2 - lat1;
    double one = pow(sin(calcLatitude / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(calcLongitude / 2), 2);
    double a = sqrt(one);
    double b;
    if (a < 1) {
        b = a;
    } else {
        b = 1.0;
    }
    double two = 2 * asin(b);
    double calculatedDistance = earthRadius * two;
    RETURN_DOUBLE(round(calculatedDistance));
    return;
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_geo_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_geo_init_globals(zend_geo_globals *geo_globals)
{
	geo_globals->global_value = 0;
	geo_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geo)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geo)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(geo)
{
#if defined(COMPILE_DL_GEO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(geo)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "geo support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	*/
}
/* }}} */

/* {{{ geo_functions[]
 *
 * Every user visible function must have an entry in geo_functions[].
 */
const zend_function_entry geo_functions[] = {
	PHP_FE(geo_get_distance, arginfo_geo_get_distance)
        PHP_FE(confirm_geo_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in geo_functions[] */
};
/* }}} */

/* {{{ geo_module_entry
 */
zend_module_entry geo_module_entry = {
	STANDARD_MODULE_HEADER,
	"geo",
	geo_functions,
	PHP_MINIT(geo),
	PHP_MSHUTDOWN(geo),
	PHP_RINIT(geo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(geo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(geo),
	PHP_GEO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(geo)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
