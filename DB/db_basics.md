### View 
  - a virtual table representing the result of a database query. 
        * the *result set* of a stored query — or map-and-reduce functions — on the data
       * the database users can query just as they would a persistent database collection object. 
       * This pre-established query command is kept in the database dictionary. 
    - Views can provide advantages over tables:
    - View can represent a subset of the data contained in a table
    - view can limit the degree of exposure of the underlying tables to the outer world: 
        * a given user may have permission to query the view, while denied access to the rest of the base table
        * provide extra security
    - Views can join and simplify multiple tables into a single virtual table
    - Views can act as aggregated tables, where the database engine aggregates data (sum, average etc.) and 
      presents the calculated results as part of the data Views can hide the complexity of data; 
    - Views take very little space to store; 
    - the database contains only the definition of a view, not a copy of all the data which it presents

materialized view (also known as snapshot)
    - a database object that contains the results of a query. 
    - Example:
        * it may be a local copy of data located remotely
        * may be a subset of the rows and/or columns of a table or join result
        * may be a summary based on aggregations of a table's data. 
    - Materialized views, which store data based on remote tables, are also known as snapshots. 
        * A snapshot can be redefined as a materialized view.

### Index 
  - A data structure that improves the speed of data retrieval operations on a database table 
  - Cost of index
    * additional writes
    * more storage space to maintain the extra copy of data.
    * Updating a table with indexes takes more time than updating a table without (because the indexes also need an update). 
	- should only create indexes on columns (and tables) that will be frequently searched against.
    
  - Why index?
    - Database softwares use indexing technology that enables *sub-linear* time lookup to improve performance
    - as linear search is inefficient for large databases.
  
  - Implementation of Index 
    - can be implemented using a variety of data structures. 
    - Popular index include balanced trees, B+ trees and hashes.
  - How index works?
    - For example, we store the data in B-tree. 
    - Initial disk reads narrowed the search range by a factor of two. 
    - That can be improved substantially by creating an auxiliary index that contains the first record in each disk block (sometimes called a sparse index). 
    - This auxiliary index would be 1% of the size of the original database, but it can be searched more quickly. 
    - Finding an entry in the auxiliary index would tell us which block to search in the main databas after searching the auxiliary index, we would have to search only that one block of the main database
    	* at a cost of one more disk read.
  - How to create index:
    - SQL:  CREATE INDEX index_name ON table_name (column_name)

Cardinality (Uniqueness) 
  - A measure of the "number of elements of the set". 
  - For example, the set A = {2, 4, 6} contains 3 elements, and therefore A has a cardinality of 3.

### Sharding
  - A database shard is a horizontal partition in a database or search engine. 
  - Each individual partition is referred to as a shard or database shard.
  - Horizontal partitioning is a database design principle whereby rows of a database table are held separately
    * rather than being split into columns (which is what normalization and vertical partitioning do)
  - Each partition forms part of a shard, which may in turn be located on a separate database server or physical location.
  - Sharding example:
    - If range-based scheme: split:
  	* A-H  ==> shard_0
  	* I-Z  ==> shard_1
  - downside: disk I/O
  - workaround: pre-split
  	* split to many logical shard
  	* distribute to fewer physical shard

Advantage of sharding
	- Total number of rows in each table in each DB is reduced. 
		* Since the tables are divided and distributed into multiple servers, 
	- Reduces index size, which generally improves search performance. 
	- DB shard can be placed on separate hardware
		* Enable a distribution of DB over a large number of machines: DB performance can be spread out over multiple machines, greatly improving performance. 
	- If DB shard is based on some real-world segmentation of the data (e.g., European customers v. American customers) 
		* it may infer the appropriate shard membership easily and automatically, and query only the relevant shard.
	- Consistent hashing is one form of automatic sharding to spread large loads across multiple smaller services and servers.
	
Disadvantages of sharding
	- Sharding a database table before it has been optimized locally causes premature complexity. 
	- Sharding should be used only when all other options for optimization are inadequate. 
	- The introduced complexity of DB sharding causes the following potential problems:
		* Increased complexity of SQL 
			- Increased bugs because the developers have to write more complicated SQL to handle sharding logic.
		* Sharding introduces complexity 
			- The sharding software that partitions, balances, coordinates, and ensures integrity can fail.
		* Single point of failure
			- Corruption of one shard due to network/hardware/systems problems causes failure of the entire table.
		* Failover servers more complex 
			- Failover servers must themselves have copies of the fleets of database shards.
		* Backups more complex 
			- Database backups of the individual shards must be coordinated with the backups of the other shards.
		* Operational complexity added 
			- Adding/removing indexes, adding/deleting columns, modifying the schema becomes much more difficult.

Vertical partitioning 
	- involves creating tables with fewer columns and using additional tables to store the remaining columns.
	- Normalization also involves this splitting of columns across tables, but vertical partitioning goes beyond that and partitions columns even when already normalized. 
	- Different physical storage might be used to realize vertical partitioning as well; storing infrequently used or very wide columns on a different device, for example, is a method of vertical partitioning. 
	- Also called "row splitting" (the row is split by its columns). 
	- A common form of vertical partitioning is to split dynamic data (slow to find) from static data (fast to find) in a table where the dynamic data is not used as often as the static. 
	
What happens during a simple write operation (Simplified version)
	1: The client sends a write command to the database (data is in client's memory).
	2: The database receives the write (data is in server's memory).
	3: The database calls the system call that writes the data on disk (data is in the kernel's buffer).
		*
	4: The operating system transfers the write buffer to the disk controller (data is in the disk cache).
		* could control by e.g., fsync().
	5: The disk controller actually writes the data into a physical media (a magnetic disk, a Nand chip, ...).
		* No control over Step 3
		
   Details about step 3:
   	- Most advanced kernels implement different layers of caching
   		* file system level caching (called the page cache in Linux) and
   		* smaller buffer cache that is a buffer containing the data that waits to be committed to the disk.
	- Buffer cache is usually never turned off because this means that every write to the file 
	  will result into data committed to the disk that is too slow for almost all the applications.
	- We can use the write system call to transfer data to the kernel buffers
	  We don't have much control about how much time this system call will take before returning successfully. 
	  The kernel write buffer is limited in size, if the disk is not able to cope with the application write bandwidth, 
	  the kernel write buffer will reach it's maximum size and the kernel will block our write. 
	  When the disk will be able to receive more data, the write system call will finally return. 
	  After all the goal is to, eventually, reach the physical media. 

Details of Step 4:
	- the kernel transfers data to the disk controller
	- By default it will try to avoid doing it too often, because it is faster to transfer it in bigger pieces. 
	  For instance Linux by default will actually commit writes after 30 seconds. 
	  This means that if there is a failure, all the data written in the latest 30 seconds can get potentially lost.
	- The POSIX API provides a family of system calls to force the kernel to write buffers to the disk: 
	  the most famous of the family is probably the fsync system call 
	  Using fsync the database system has a way to force the kernel to actually commit data on disk.
	   * very expensive operation: fsync will initiate a write operation every time it is called and 
	    there is some data pending on the kernel buffer. 
	   * Fsync() also blocks the process for all the time needed to complete the write, 
	     on Linux it will also block all the other threads that are writing against the same file.
