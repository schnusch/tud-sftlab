/**
 * The class MyStack is written for use within the computer lab cours in dependable systems in winter term 2004/2005.
 * It is an example that shows some problems that arise in connection with the use of excetions.
 * <br> Author: Gert Pfeifer, Dresden University of Technology, Systems Engineering Group
 * @author: Gert Pfeifer, Dresden University of Technology, Systems Engineering Group
 */public class MyStack{

    private int counter;
    private Object[] stack;

    public MyStack(int size){
	counter = 0;
	stack = new Object[size];
    }

/**
 * This method is used to get the current number of elements.
 * @return number of elements on MyStack.
 */
    public int getCounter(){return counter;}

/**
 * This method gives a String representation of the MyStack object.
 * @return String representation of the Object
 */
    public String toString(){
	String s = new String();
	for(int i=0;i<counter;i++){
	    s=s.concat(stack[i]+";;");
	}
	return s;
    }

/**
 * This method is used to add an object on the MyStack.
 * @param p the object to put on the MyStack
 */
    public void addElement(Object p){
	
	stack[counter]=p;
	counter++;
    }

/**
 * This method is used to resize the MyStack. It is useful if <i>addElement</i> returns an <i>ArrayIndexOutOfBoundsException</i>. 
 */
    public void resizeStack(){
	
	int newSize = stack.length + 3;
	Object[] newStack = new Object[newSize];
	System.arraycopy(stack,0,newStack,0,stack.length);
	stack = newStack;
	}

	public static void main(String[] args)
	{
		MyStack ms = new MyStack(1);
		System.out.println(ms);
		String[] ss = {"a", "b", "c", "d", "e"};
		for(int i = 0; i < ss.length; i++)
		{
			try
			{
				ms.addElement(ss[i]);
			}
			catch(ArrayIndexOutOfBoundsException e)
			{
				ms.resizeStack();
				ms.addElement(ss[i]);
			}
			System.out.println(ms);
		}
	}
}
