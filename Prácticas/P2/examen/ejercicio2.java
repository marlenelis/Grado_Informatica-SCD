import monitor.*;
class Impresora extends AbstractMonitor{
	private  Condition cola_usuario ;//vble condicion 
	private  Condition cola_super ;//vble condicion 
	private Condition estado;

	
	public Impresora(){
		
		this.cola_usuario= makeCondition();
		this.cola_super = makeCondition();
		
		this.estado= makeCondition();
		
	}
//=====================================================================
	public void imprimir(int h){
	//modificar la variable estado del monitor par indicar que hay un nuevo usuario que quiere imprimir
	//si hay usuarios imprimiendo o esperando para imprimir -> bloqueame
enter();
	if(!estado.isEmpty() ){
		
		cola_usuario.await();
		cola_super.await();
		
		System.out.println("esperando");
	}
		System.out.println(" Imprimiendo Usuario "+h);

		estado.await();
	leave();	
}
//=====================================================================	
	public void terminarImprimir(int h){

		//Modificar la variable estado del monitor para indicar que el usuario ha imprimido por lo que hay un usuario menos utilizando la impresora
//Si el supèrusuario esta esperando par imprimir -> desbloquear el superusuario
//Si no desbloquear el proximo usuario que esta esperando para imprimir
	enter();
		estado.signal();

		if(!cola_super.isEmpty())//si la cola se super no esta vacia
			 cola_super.signal();
		else
			cola_usuario.signal();

		System.out.println(" Imprimiendo Super ");
leave();
	}
//=====================================================================
	public void imprimirSuperU(){
	enter();
	
		if(!estado.isEmpty() ){
			System.out.println(" Super esperando");
			cola_super.await();
		}
		System.out.println(" Imprimiendo Super ");
		estado.await();
	leave();
	}
//=====================================================================		
	
	public void terminarImprimirSuperU(){
	enter();
	
		estado.signal();

			cola_usuario.signal();
	leave();
	}
}
//============================================================================

class Usuario  implements Runnable{

	private Impresora monitor;
	private int ih;
	public Thread th;
	
	public Usuario (Impresora p_monitor, int p_ih){
		
		String name = "Usuario "+p_ih;
		this.monitor = p_monitor;
		this.th = new Thread(this, name);
	}

	 @Override
    	public void run() {
		try{ 
		    while(true){ 
		       
		        aux.dormir_max(1500);
			monitor.imprimir(ih);
			aux.dormir_max(1000);//simula que el usuario esta imprimiendo
			monitor.terminarImprimir(ih);
		    }

		}catch( Exception e ) { System.err.println("Excepcion en run Usuario: " );} 
    	}
}


//============================================================================


class SuperUsuario implements Runnable{

	private Impresora monitor;	
	public Thread thr;
	
	public SuperUsuario(Impresora p_monitor){
		this.monitor = p_monitor;
		this.thr = new Thread(this, "SuperUsuario");
	}

	 @Override
    public void run() {
        try{ 
            while(true){ 
               
                aux.dormir_max(3500);
		monitor.imprimirSuperU();
		aux.dormir_max(2000);//simula que el superUsuario esta imprimiendo
		monitor.terminarImprimirSuperU();
            }

        }catch( Exception e ) { System.err.println("Excepcion en run Super: ");} 
    }
}

//============================================================================


public class ejercicio2 {
    
    public static void main(String[] args) {
        Impresora impresora = new Impresora(); //monitor

	int nUsuarios = Integer.parseInt(args[0]);
	System.out.println(nUsuarios);
//int nUsuarios =7;
	Usuario usuarios[] = new Usuario[ nUsuarios];
       
	SuperUsuario super_ = new SuperUsuario(impresora);//hebra superUsuario
	super_.thr.start();
        for(int i=0; i<nUsuarios;++i){
            usuarios[i]= new Usuario(impresora, i);//hebras usuarios
            usuarios[i].th.start();
        }   
        
	
    }
    
}
