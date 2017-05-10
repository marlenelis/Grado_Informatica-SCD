import monitor.*;
import java.util.Random;
/**
 * Barberia.java
 * @author Marlene E. Vásquez Calero
 * @version 1.0
 * @date 18-nov-2016
 * 
 * compilar:  javac monitores/*.java
 *	      javac barberoDurmiente.java 
 *        
 * uso:	 java barberoDurmiente
 */
//============================================================================
// class Barberia: 
//----------------------------------------------------------------------------
class Barberia extends AbstractMonitor{

    private  Condition barbero ;//vble condicion 
    private  Condition sala;    //vble condicion 
    private  Condition silla;   //vble condicion 
    private static int contador;
    private static int n;
    private boolean cortar;

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
            
	     if(sala.count()<4){ 
                   
            if( !this.silla.isEmpty()){//si hay un cliente sentado  
                         
                aux.mensaje("esperando "+ sala.count() );                               
                this.sala.await();

            }                            
                this.barbero.signal(); //despierta al barbero           
                this.silla.await();// sienta al cliente            
                aux.mensaje(" sentado");

         }else

            aux.mensaje("hay "+sala.count()+ " clientes esperando me voy");

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
            }
           
            this.sala.signal(); //despierta al siguiente
             System.out.println("Barbero: cortando el pelo");

        leave();
    }
    //------------------------------------------------------------------------
    //invocado por el barbero para indicar que ha terminado de cortar el pelo
    //------------------------------------------------------------------------
    public void finCliente(){
        enter();  

        
		
        this.contador+=1;
      
		if(this.contador == 5){
			 System.out.println("\tHoy estoy muy ocupado");	
			 this.contador = 0;	
		}else System.out.println("\tBarbero: fin cliente ");
	
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
    
    public Cliente(Barberia bb , int i){

        this.barberia = bb;       
        this.thr = new Thread(this,"cliente " +i);
        
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
public class barbero_ex {
    
    public static void main(String[] args) {
        Barberia barberia = new Barberia(); //monitor
        Barbero barbero = new Barbero(barberia,"Barbero");//hebra barbero
        barbero.thr.start();
        
        Cliente clientes[] = new Cliente[7];         
        for(int i=0; i<7;++i){
            clientes[i]= new Cliente(barberia ,i);//hebras cliente
            clientes[i].thr.start();
        }   
        
    }
    
}
