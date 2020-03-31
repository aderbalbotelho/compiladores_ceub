import java.util.*;

class Nomes {
  private Map<String,Integer> _nomes;
  public Nomes() {
    _nomes = new HashMap<String,Integer>(); }

  public int func(String a, String b) {
    int res = _nomes.get(a);
    int val = _nomes.get(b);

    _nomes.put(b, val + res);

    return res + val;
  }
  public static void main(String[] args) {
    (new Nomes()).print();
  }
  public void print() {
    _nomes.put("x", 12);
    _nomes.put("y", 15);
    _nomes.put("z", 17);

    System.out.println(func("x", "y"));
    System.out.println(func("z", "y"));
    System.out.println(func("y", "z"));
    System.out.println(func("x", "z"));
  }
}
