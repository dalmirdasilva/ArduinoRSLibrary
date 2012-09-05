/**
 * SDCC - PIC resource system
 * 
 * rs_util.h
 * 
 * Util lib for rs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_UTIL_H__
#define __SDCC_RS_UTIL_H__ 1

#include "rs_io.h"
#include "rs.h"

/**
 * Write a resource system table to disc
 * 
 * @param driver
 * @param rs
 */
void _rs_write_rs_to_disc(rs_driver_t driver, rs_t *rs);

/**
 * Read a resource system table from disc
 * 
 * @param driver
 * @param rs
 */
void _rs_read_rs_from_disc(rs_driver_t driver, rs_t *rs);

/**
 * Allocate a free cluster from disc if any
 * 
 * @param rs
 * @return 
 */
rs_memory_address_t _rs_alloc_cluster(rs_t *rs);

/**
 * Test if the given cluster is free
 * 
 * @param rs
 * @param cluster
 * @return 
 */
uint8_t _rs_is_free_cluster(rs_t *rs, rs_cluster_t cluster);

/**
 * Format a given cluster
 * 
 * @param rs
 * @param cluster
 */
void _rs_format_cluster(rs_t *rs, rs_cluster_t cluster);

/**
 * Free a given cluster
 * 
 * @param rs
 * @param cluster
 */
void _rs_free_cluster(rs_t *rs, rs_cluster_t cluster);

/**
 * Create a chain between two clusters
 * 
 * @param rs
 * @param prev_cluster
 * @param next_cluster
 */
void _rs_create_cluster_chain(rs_t *rs, rs_cluster_t prev_cluster, rs_cluster_t next_cluster);

/**
 * Convert resource code to rd
 * 
 * @param resource
 */
#define _rs_resource_code_to_resource_descriptor(resource_code)         (rs_resource_descriptor_t)(resource_code)

/**
 * Convert rd to resource code
 * 
 * @param resource
 */
#define _rs_resource_descriptor_to_resource_code(resource_descriptor)   (rs_resource_code_t)(resource_descriptor)

/**
 * Convert cluster to address
 * 
 * @param resource
 */
#define _rs_cluster_to_address(rs, cluster)                             (rs_memory_address_t)(rs->cluster_table_address + (cluster * rs->sizeof_cluster))

/**
 * Convert address to cluster
 * 
 * @param resource
 */
#define _rs_address_to_cluster(rs, address)                             (rs_cluster_t)((address - rs->cluster_table_address) / rs->sizeof_cluster)

/**
 * Convert rd to address
 * 
 * @param resource
 */
#define _rs_resource_descriptor_to_address(rs, resource_descriptor)     (rs_memory_address_t)((resource_descriptor * rs->sizeof_resource_descriptor) + rs->resource_descriptor_table_address)

/**
 * Convert address to rd
 * 
 * @param resource
 */
#define _rs_address_to_resource_descriptor(rs, address)                 (rs_resource_descriptor_t)((address - rs->resource_descriptor_table_address) / rs->sizeof_resource_descriptor)

/**
 * Check if the end-of-resource is reached and set or clear the respecitve flag
 * 
 * @param resource
 */
void _rs_check_for_eor_reached(rs_resource_t *resource);

/**
 * Test the end-of-resource flag
 * 
 * @param resource
 * @return 
 */
uint8_t _rs_is_eor_reached(rs_resource_t *resource);

/**
 * Check if we are at the end of resource, if yes alloc another cluster and 
 * manage the new pointers
 * 
 * @param rs
 * @param resource
 * @return 
 */
uint8_t _rs_check_for_availability(rs_t *rs, rs_resource_t *resource);

/**
 * Move the current position ahead 'offset' bytes
 * 
 * @param rs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _rs_move_current_position_ahead(rs_t *rs, rs_resource_t *resource, rs_seek_int_t offset);

/**
 * Move the current position back 'offset' bytes
 * 
 * @param rs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _rs_move_current_position_back(rs_t *rs, rs_resource_t *resource, rs_seek_int_t offset);

/**
 * Free a resource description
 * 
 * @param rs
 * @param resource_descriptor
 */
void _rs_format_resorce_descriptor(rs_t *rs, rs_resource_descriptor_t resource_descriptor);

/**
 * Test if given driver is mouted
 * 
 * @param driver
 * @return 
 */
uint8_t _rs_is_driver_monted(rs_driver_t driver);

/**
 * Set/clear given driver as mouted
 * 
 * @param driver
 * @param is
 */
void _rs_set_driver_monted(rs_driver_t driver, uint8_t is);

/**
 * Close all resources
 * 
 * @param rs
 */
void _rs_free_resource_descriptors(rs_t *rs);

/**
 * Close a single resources
 * 
 * @param rs
 * @param resource_descriptor
 */
void _rs_free_resource_descriptor(rs_t *rs, rs_resource_descriptor_t resource_descriptor);

/**
 * Decrease free cluster
 * 
 * @param rs
 * @param resource
 */
#define _rs_decrease_free_clusters(rs, n)   { \
                                                rs->free_clusters -= n; \
                                                _rs_write_rs_to_disc(rs->driver, rs); \
                                            }

/**
 * Increase free cluster
 * 
 * @param rs
 * @param resource
 */
#define _rs_increase_free_clusters(rs, n)   { \
                                                rs->free_clusters += n; \
                                                _rs_write_rs_to_disc(rs->driver, rs); \
                                            }

/**
 * Free resource cluster
 * 
 * @param rs
 * @param resource
 */
void _rs_format_resource_clusters(rs_t *rs, rs_resource_t *resource);

/**
 * Format a chain of clusters
 * 
 * @param rs
 * @param cluster
 * @return 
 */
uint8_t _rs_format_clusters_chain(rs_t *rs, rs_cluster_t cluster);

/**
 * Get the previous cluster by a cluster
 * 
 * @param rs
 * @return 
 */
#define _rs_prev_cluster_by_cluster(rs, cluster)                _rs_prev_cluster_by_cluster_address(rs, _rs_cluster_to_address(rs, cluster))

/**
 * Get the next cluster by a cluster
 * 
 * @param rs
 * @return 
 */
#define _rs_next_cluster_by_cluster(rs, cluster)                _rs_next_cluster_by_cluster_address(rs, _rs_cluster_to_address(rs, cluster))

/**
 * Get the previous cluster by a cluster address
 * 
 * @param rs
 * @return 
 */
#define _rs_prev_cluster_by_cluster_address(rs, address)        (rs_cluster_t)(_rs_io_read(rs->driver, CLUSTER_ADDRESS_TO_PREV(address)))

/**
 * Get the next cluster by a cluster address
 * 
 * @param rs
 * @return 
 */
#define _rs_next_cluster_by_cluster_address(rs, address)        (rs_cluster_t)(_rs_io_read(rs->driver, CLUSTER_ADDRESS_TO_NEXT(address)))

/**
 * Calculates and evaluate the rs attributes
 * 
 * @param rs
 * @return 
 */
uint8_t _rs_has_invalid_attributes(rs_t *rs);

#endif // __SDCC_RS_UTIL_H__
