using System.Runtime.ConstrainedExecution;
using System.Xml;

namespace lr5cs
{
    public class Node
    {
        public int Value;
        public Node Next;

        public Node(int value, Node next = null)
        {
            Value = value;
            Next = next;
        }
    }
    internal class Program
    {
        static int Input(string s)
        {
            int v;
            while (true)
            {
                Console.WriteLine(s);
                if (int.TryParse(Console.ReadLine(), out v))
                    break;
                else
                    Console.WriteLine("Не распознанное число");

            }
            return v;
        }

        static void Add(Node cur, Node next, Node head)
        {
            bool isAdd = true;
            while (isAdd)
            {
                int v = Input("Введите значение");
                cur.Value = v;
                int c = Input("0 - выход 1 - добавить элемент 2 - добавить цикл");
                switch (c)
                {
                    case 0:
                        isAdd = false;
                        break;
                    case 1:
                        next = new Node(0);
                        cur.Next = next;
                        cur = next;
                        break;
                    case 2:
                        int n;
                        while (true)
                        {
                            Console.Write("Введите номер с которым соединить элемент - ");
                            if (int.TryParse(Console.ReadLine(), out n))
                            {
                                Node node = head;
                                while (node != null && node.Value != n)
                                {
                                    node = node.Next;
                                }
                                if (node == null)
                                {
                                    Console.WriteLine("Не найден элемент с введенным номером");
                                    continue;
                                }
                                if (node.Value == n)
                                {
                                    cur.Next = node;
                                    break;
                                }
                            }
                            else
                                Console.WriteLine("Не распознанное число");
                        }
                        isAdd = false;
                        break;
                }
            }
        }

        

        static void Main(string[] args)
        {
            Console.WriteLine("Ввод списка");
            Node cur = new Node(0), next = null, head = cur;
            //Add(cur, next, head);
            int p = Input("Выберите способ проверки списка\r\n1 - простой перебор 2 - два указателя 3 - реверс списка");
            bool hasCycle = false;
            switch(p)
            {
                case 1:
                    hasCycle = HasCycleNaive(head, 50);
                    break;
                case 2:
                    hasCycle = HasCycleFloyd(head);
                    break;
                case 3:
                    hasCycle = HasCycleByReversing(head);
                    break;
            }
            if (hasCycle)
                Console.WriteLine("Цикл есть");
            else
                Console.WriteLine("Цикла нет");
        }

        public static bool HasCycleNaive(Node head, int n)
        {
            if (head == null) return false;

            for (Node p = head; p != null; p = p.Next)
            {
                Node q = p.Next;

                for (int step = 0; step < n && q != null; step++)
                {
                    if (q == p)
                        return true; // вернулись в p — цикл

                    q = q.Next;
                }
            }

            return false;
        }

        public static bool HasCycleFloyd(Node head)
        {
            Node slow = head;
            Node fast = head;

            while (fast != null && fast.Next != null)
            {
                slow = slow.Next;          // 1 шаг
                fast = fast.Next.Next;     // 2 шага

                if (slow == fast)
                    return true;           // указатели встретились — цикл
            }

            return false;                  // дошли до конца — цикла нет
        }

        public static bool HasCycleByReversing(Node head)
        {
            if (head == null) return false;

            // ---------- Фаза 1: разворот ----------
            Node prev = null;
            Node curr = head;
            bool hasCycle = false;

            while (curr != null)
            {
                // Вернулись к голове (и это не самый первый шаг) — значит, цикл через head
                if (curr == head && prev != null)
                {
                    hasCycle = true;
                    break;
                }

                Node next = curr.Next;
                curr.Next = prev;   // разворачиваем ссылку
                prev = curr;
                curr = next;
            }

            // prev — последний обработанный узел. В циклическом случае это «хвост»,
            // который в исходном списке указывал на head.
            Node tail = prev;

            // ---------- Фаза 2: восстановление ----------
            Node restorePrev = null;
            Node restoreCurr = tail;

            while (restoreCurr != null)
            {
                Node next = restoreCurr.Next;
                restoreCurr.Next = restorePrev;
                restorePrev = restoreCurr;
                restoreCurr = next;
            }

            // Если был цикл, нужно замкнуть хвост обратно на голову
            if (hasCycle && tail != null)
            {
                tail.Next = head;
            }

            return hasCycle;
        }
    }
}
