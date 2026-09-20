/**
 * @file error.h
 * @brief Shared error codes returned by all galxlib functions.
 */

#ifndef GALXLIB_ERROR_H
#define GALXLIB_ERROR_H

/**
 * @enum GalxlibError
 * @brief The error codes returned by galxlib functions.
 */
enum GalxlibError
{
    GLX_SUCCESS = 0,                              /**< (0) Successful execution of the called function. */
    GLX_ERR_NULL_ARGUMENT = 1,                    /**< (1) One or more arguments are NULL. */
    GLX_ERR_MEMORY_ALLOCATION = 2,                /**< (2) Failed to allocate or reallocate memory. */
    GLX_ERR_INVALID_ARGUMENT_DIMENTIONS = 3,      /**< (3) The dimentions of one or more arguments, either alone or in their combination, do not match the expectations of the function. */
    GLX_ERR_INDEX_OUT_OF_BOUNDS = 4,              /**< (4) The targeted index is outside of the boundaries of the data structure. */
    GLX_ERR_NULL_DESTRUCTOR = 5,                  /**< (5) The destructor function pointer of the data structure is NULL. */
    GLX_ERR_TYPE_MISMATCH = 6,                    /**< (6) The type of the data structure does not match the expected type. */
    GLX_ERR_TYPE_UNKNOWN = 7,                     /**< (7) The type of the data structure is unknown. */
    GLX_ERR_ITEM_SIZE_DETERMINATION = 8,          /**< (8) The size of the item in the data structure could not be determined. */
    GLX_ERR_NULL_ARRAY = 9,                       /**< (9) The underlying array is NULL. This is the structure member containig the elements - e.g. int *DynamicArray::int_arr for DA_INT type (DynamicArrayType). */
    GLX_ERR_NULL_STR = 10,                        /**< (10) The underlying character array of the String is NULL. This is the structure member containig the characters - e.g. char *String::str. */
    GLX_ERR_LOOP_MAX_LIMIT = 11,                  /**< (11) The maximum number of iterations in a loop has been reached. This is a safety measure to prevent infinite loops. */
    GLX_ERR_ZERO_LENGTH = 12,                     /**< (12) The length of the underlying array is zero. This is a safety measure to prevent operations on empty strings, for example. */
    GLX_ERR_KEY_MAX_LENGTH = 13,                  /**< (13) The length of the key in a hash map is greater than the maximum allowed length. */
    GLX_ERR_KEY_EMPTY = 14,                       /**< (14) The length of the given key is 0. */
    GLX_ERR_KEY_COPY = 15,                        /**< (15) An error occured while coping the key using strcpy_s. */
    GLX_ERR_FULL = 16,                            /**< (16) The hash map is full. */
    GLX_ERR_MAX_CAPACITY = 17,                    /**< (17) The number of Entries in the HashMap - HashMap::n_ent - has reached the limit of HASH_MAP_MAX_CAPACITY. */
    GLX_ERR_PATTERN_COMPILATION = 18,             /**< (18) Failed to compile the regex pattern using the regcomp function. */
    GLX_ERR_NO_MATCH = 19,                        /**< (19) The given @ref RegexContainer has not matched a string (character array or String). */
    GLX_ERR_MISSING_COMPILED_PATTERN_BUFFER = 20, /**< (20) The given @ref RegexContainer does not contain a compiled pattern buffer. The structure member, pointing to a regex_t structure, is NULL (container->regex == NULL). */
    GLX_ERR_MISSING_MATCH_GROUPS = 21,            /**< (21) The given @ref RegexContainer does not contain any match groups. The structure member, pointing to a regmatch_t array, is NULL (container->groups == NULL). */
    GLX_ERR_REGEX_EXECUTION = 22,                 /**< (22) The regexec function has returned an error code (different than 0 and REG_NOMATCH), while trying to match. */
};

#endif

/**
 * Mapping of old error coders to new error codes for backward compatibility.
 * NEW => OLD
 *
 * GLX_SUCCESS => GLX_SUCCESS, GLX_SUCCESS, GLX_SUCCESS, GLX_SUCCESS
 * GLX_ERR_NULL_ARGUMENT => GLX_ERR_NULL_ARGUMENT, GLX_ERR_NULL_ARGUMENT, GLX_ERR_NULL_ARGUMENT, GLX_ERR_NULL_ARGUMENT
 * GLX_ERR_MEMORY_ALLOCATION => GLX_ERR_MEMORY_ALLOCATION, GLX_ERR_MEMORY_ALLOCATION, GLX_ERR_MEMORY_ALLOCATION, GLX_ERR_MEMORY_ALLOCATION
 * GLX_ERR_INVALID_ARGUMENT_DIMENTIONS => GLX_ERR_INVALID_ARGUMENT_DIMENTIONS, GLX_ERR_INVALID_ARGUMENT_DIMENTIONS, GLX_ERR_INVALID_ARGUMENT_DIMENTIONS, GLX_ERR_INVALID_ARGUMENT_DIMENTIONS
 * GLX_ERR_INDEX_OUT_OF_BOUNDS => GLX_ERR_INDEX_OUT_OF_BOUNDS, HM_ERR_INDEX_OUT_OF_BOUNDS, RC_ERR_INDEX_OUT_OF_BOUNDS, GLX_ERR_INDEX_OUT_OF_BOUNDS
 * GLX_ERR_NULL_DESTRUCTOR => GLX_ERR_NULL_DESTRUCTOR, GLX_ERR_NULL_DESTRUCTOR, RC_ERR_NULL_DESTRUCTOR, STR_ERR_NULL_DESTRUCTOR
 * GLX_ERR_TYPE_MISMATCH => GLX_ERR_TYPE_MISMATCH, HM_ERR_TYPE_MISMATCH, RC_ERR_TYPE_MISMATCH, STR_ERR_TYPE_MISMATCH
 * GLX_ERR_TYPE_UNKNOWN => GLX_ERR_TYPE_UNKNOWN, HM_ERR_TYPE_UNKNOWN, RC_ERR_TYPE_UNKNOWN, STR_ERR_TYPE_UNKNOWN
 * GLX_ERR_ITEM_SIZE_DETERMINATION => GLX_ERR_ITEM_SIZE_DETERMINATION, HM_ERR_ITEM_SIZE_DETERMINATION, RC_ERR_ITEM_SIZE_DETERMINATION, STR_ERR_ITEM_SIZE_DETERMINATION
 * GLX_ERR_NULL_ARRAY => GLX_ERR_NULL_ARRAY, HM_ERR_NULL_ARRAY, RC_ERR_NULL_ARRAY, STR_ERR_NULL_ARRAY
 * GLX_ERR_NULL_STR => DA_ERR_NULL_STR, HM_ERR_NULL_STR, RC_ERR_NULL_STR, GLX_ERR_NULL_STR
 * GLX_ERR_LOOP_MAX_LIMIT => DA_ERR_LOOP_MAX_LIMIT, HM_ERR_LOOP_MAX_LIMIT, RC_ERR_LOOP_MAX_LIMIT, GLX_ERR_LOOP_MAX_LIMIT
 * GLX_ERR_ZERO_LENGTH => DA_ERR_ZERO_LENGTH, HM_ERR_ZERO_LENGTH, RC_ERR_ZERO_LENGTH, GLX_ERR_ZERO_LENGTH
 * GLX_ERR_KEY_MAX_LENGTH => GLX_ERR_KEY_MAX_LENGTH
 * GLX_ERR_KEY_EMPTY => GLX_ERR_KEY_EMPTY
 * GLX_ERR_KEY_COPY => GLX_ERR_KEY_COPY
 * GLX_ERR_FULL => GLX_ERR_FULL
 * GLX_ERR_MAX_CAPACITY => GLX_ERR_MAX_CAPACITY
 * GLX_ERR_PATTERN_COMPILATION => GLX_ERR_PATTERN_COMPILATION
 * GLX_ERR_NO_MATCH => GLX_ERR_NO_MATCH
 * GLX_ERR_MISSING_COMPILED_PATTERN_BUFFER => GLX_ERR_MISSING_COMPILED_PATTERN_BUFFER
 * GLX_ERR_MISSING_MATCH_GROUPS => GLX_ERR_MISSING_MATCH_GROUPS
 * GLX_ERR_REGEX_EXECUTION => GLX_ERR_REGEX_EXECUTION
 **/