#!/usr/bin/ruby



if ARGV[0].to_i == 0
    puts "Usage: init.rb {size}"
    exit(0)
end

available = ARGV[0].to_i * 1024

resource_descriptor_table_address = 32
sizeof_resource_descriptor = 4
resource_descriptor_count = 32
sizeof_resource_descriptor_table = sizeof_resource_descriptor * resource_descriptor_count
cluster_table_address = sizeof_resource_descriptor_table + resource_descriptor_table_address

sizeof_cluster_data = 128
sizeof_cluster_control = 2
sizeof_cluster = sizeof_cluster_data + sizeof_cluster_control

cluster_space = available - cluster_table_address
cluster_count = (cluster_space / sizeof_cluster).to_i
free_clusters = cluster_count

sizeof_cluster_table = cluster_count * sizeof_cluster
memory_size = sizeof_cluster_table + cluster_table_address

puts "rs->memory_size = 0x#{memory_size.to_s(16)}; //#{memory_size};"
puts "rs->resource_descriptor_table_address = 0x#{resource_descriptor_table_address.to_s(16)}; //#{resource_descriptor_table_address};"
puts "rs->cluster_table_address = 0x#{cluster_table_address.to_s(16)}; //#{cluster_table_address};"
puts "rs->sizeof_resource_descriptor_table = 0x#{sizeof_resource_descriptor_table.to_s(16)}; //#{sizeof_resource_descriptor_table};"
puts "rs->sizeof_cluster_table = 0x#{sizeof_cluster_table.to_s(16)}; //#{sizeof_cluster_table};"
puts "rs->sizeof_resource_descriptor = 0x#{sizeof_resource_descriptor.to_s(16)}; //#{sizeof_resource_descriptor};"
puts "rs->sizeof_cluster = 0x#{sizeof_cluster.to_s(16)}; //#{sizeof_cluster};"
puts "rs->resource_descriptor_count = 0x#{resource_descriptor_count.to_s(16)}; //#{resource_descriptor_count};"
puts "rs->cluster_count = 0x#{cluster_count.to_s(16)}; //#{cluster_count};"
puts "rs->sizeof_cluster_data = 0x#{sizeof_cluster_data.to_s(16)}; //#{sizeof_cluster_data};"
puts "rs->sizeof_cluster_control = 0x#{sizeof_cluster_control.to_s(16)}; //#{sizeof_cluster_control};"
puts "rs->free_clusters = 0x#{free_clusters.to_s(16)}; //#{free_clusters};"
puts "rs->flags = 0x00; //0;"
