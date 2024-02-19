import std.algorithm;
import std.container.binaryheap;
import std.conv;
import std.stdio;
import std.string;

class Node
{
  long num;
  Node next;
  Node prev;

  this (long num)
  {
    this.num = num;
  }

  override string toString() const
  {
    return to!string(num);
  }
}

class CircularList
{
  Node head;
  size_t length;

  final Node insert(long num)
  {
    length++;
    Node newNode = new Node(num);
    if (head is null)
      {
	newNode.next = newNode;
	newNode.prev = newNode;
	head = newNode;
      }
    else
      {
	Node last = head.prev;
	newNode.next = head;
	head.prev = newNode;
	newNode.prev = last;
	last.next = newNode;
      }
    return newNode;
  }

  private Node insertAfter(Node node, Node after)
  {
    node.prev = after;
    node.next = after.next;

    after.next = node;
    node.next.prev = node;

    return node;
  }

  private Node insertBefore(Node node, Node before)
  {
    node.next = before;
    node.prev = before.prev;

    before.prev = node;
    node.prev.next = node;

    return node;
  }

  final void shift(Node node, long offset)
  {
    if (offset != 0)
      {
	// Lift the node out of the list
	node.prev.next = node.next;
	node.next.prev = node.prev;
      }

    // Get the node after which the given node will be moved to
    Node curr = node;
    if (offset > 0)
      {
	offset = offset % (length - 1);
	for (int i = 0; i < offset; ++i)
	  curr = curr.next;
	insertAfter(node, curr);
      }
    else if (offset < 0)
      {
	offset = -offset % (length - 1);
	for (int i = 0; i < offset; ++i)
	  curr = curr.prev;
	insertBefore(node, curr);
      }
  }

  final Node find(long num)
  {
    Node curr = head;
    do
      {
	if (curr.num == num)
	  break;
	curr = curr.next;
      }
    while (curr != head);

    if (curr == head)
      return null;
    else
      return curr;
  }

  override string toString()
  {
    string result;
    Node curr = head;
    do
      {
	result ~= curr.toString() ~ " ";
	curr = curr.next;
      }
    while (curr != head);
    return result;
  }
}

long coordinateSum(CircularList numList, Node[] numbers)
{
  auto offsets = heapify!"a > b"([1000%numbers.length,
				  2000%numbers.length,
				  3000%numbers.length]);
  long sum;
  Node curr = numList.find(0);
  for (int i; i < numbers.length && !offsets.empty; ++i)
    {
      if (i == offsets.front)
	{
	  sum += curr.num;
	  offsets.popFront();
	}
      curr = curr.next;
    }
  return sum;
}

void main()
{
  enum decryptionKey = 811_589_153;
  Node[] numbers1, numbers2;
  CircularList numList1 = new CircularList(), numList2 = new CircularList();
  string line;
  while ((line = readln().strip) !is null)
    {
      numbers1 ~= numList1.insert(to!long(line));
      numbers2 ~= numList2.insert(to!long(line) * decryptionKey);
    }

  //
  // Part 1
  //
  foreach (node; numbers1)
    numList1.shift(node, node.num);
  writeln("Part 1: ", coordinateSum(numList1, numbers1));

  //
  // Part 2
  //
  for (int i; i < 10; ++i)
    {
      foreach (node; numbers2)
	numList2.shift(node, node.num);
    }
  writeln("Part 2: ", coordinateSum(numList2, numbers2));
}
