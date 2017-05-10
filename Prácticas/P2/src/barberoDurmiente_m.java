import monitor.*;
import java.util.Random;
/**
 * Barberia_m.java
 * @author Marlene E. Vásquez Calero
 * @version 1.0
 * @date 18-nov-2016
 * 
 * compilar:  javac monitores/*.java
 *	      javac barberoDurmiente__m.java 
 *        
 * uso:	 java barberoDurmiente_m
 */
//============================================================================
// class Barberia: 
//----------------------------------------------------------------------------
class Barberia extends AbstractMonitor{

    private  Condition barbero ;//vble condicion 
    private  Condition sala;    //vble condicion 
    private  Condition silla;   //vble condicion 
    private static int contador;
    private int cortar;
    public Barberia(){
       	barbero = makeCondition();
    	sala    = makeCondition();
    	silla   = makeCondition();
		contador = 0;
    }
    //------------------------------------------------------------------------
    //invocado por los clientes para cortarse el pelo
    //------------------------------------------------------------------------
    public void cortarPelo(){
        enter();

	    this.cortar = (int) (Math.random()*2.0);//0 (no),1 (si)
		

	    if(cortar == 1){//decide cortarse el pero

		    if( !this.silla.isEmpty()){//si hay un cliente sentado
		        System.out.println("Cliente: esperando"); 
		        this.sala.await();
		    }
                        
		    this.barbero.signal(); //despierta al barbero
		    this.silla.await();// sienta al cliente 
		    System.out.println("Cliente: sentado"); 
             }         
        leave();        
    }
    //------------------------------------------------------------------------
    //invocado por el barbero para esperar (si procede) a un nuevo cliente y 
    //sentarlo para el corte
    //------------------------------------------------------------------------
    public void siguienteCliente(){
        enter();

            if(this.silla.isEmpty() && this.sala.isEmpty()){//si no hay clientes en la sala y tampoco en la silla 
                System.out.println("Barbero: durmiendo");//
                this.barbero.await();//barbero espera
            }else if(this.silla.isEmpty()){ // si la silla esta vacia
                System.out.println("Barbero: esperando a que se siente el cliente");	//barbero espera
		 	    this.barbero.await();	
		    }

            System.out.println("Barbero: cortando el pelo");
            this.sala.signal(); //despierta al siguiente

        leave();
    }
    //------------------------------------------------------------------------
    //invocado por el barbero para indicar que ha terminado de cortar el pelo
    //------------------------------------------------------------------------
    public void finCliente(){
        enter();  

            System.out.println("Barbero: fin cliente ");
		this.contador++;
		if(this.contador == 3){
			 System.out.println("\tEstoy harto de tantas visitas");	
			 this.contador = 0;	
		}
	
            this.silla.signal();//desbloquea silla    
		

        leave();        
    }
}
//------------------------------------------------------------------------
 class Barbero implements Runnable
 {
     Barberia barberia;
     Thread thr;
     public  Barbero( Barberia bb, String name){
         this.barberia = bb;
         this.thr = new Thread(this, name);

     }
   //--------------------------------
    @Override
    public void run() {
        try{ 
            while(true){ 
                this.barberia.siguienteCliente();
                aux.dormir_max(500);//el barbero esta cortando el pelo
                barberia.finCliente();
            }

        }catch( Exception e ) { System.err.println("Excepcion en run barbero: " + e);} 
    }
}
//------------------------------------------------------------------------
class Cliente implements Runnable{

    public Thread thr;
    private Barberia barberia;   
    
    public Cliente(Barberia bb){

        this.barberia = bb;       
        this.thr = new Thread(this,"cliente");
        
    }
    @Override
    public void run() {
        try{ 
            while(true){
		
                   this.barberia.cortarPelo();//el cliente espera si procede y se corta el pelo
                   aux.dormir_max(5000);// el cliente esta fuera de la barberia un tiempo 
		           
            }

        }catch( Exception e ) { System.err.println("Excepcion en run cliente: " + e);}
    }
}                              
//------------------------------------------------------------------------
public class barberoDurmiente_m {
    
    public static void main(String[] args) {
        Barberia barberia = new Barberia(); //monitor
        Thread barbero = new Thread(new Barbero(barberia,"Barbero"));//hebra barbero
        barbero.start();
        
        Thread clientes[] = new Thread[5];         
        for(int i=0; i<5;++i){
            clientes[i]= new Thread(new Cliente(barberia));//hebras cliente
            clientes[i].start();
        }   
        
    }
    
}
