/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package achive;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


/**
 *
 * @author M0C
 */
public class ArchiveRetrieve 
{
    private String desPath;
    private Scanner input=null;
    
    public ArchiveRetrieve(String destinationPath)
    {
        desPath=destinationPath;
        
    }
    public ArchiveEntity[] get_Sorted_Archive_List(String hostChatter)
    {
        ArchiveEntity[] temp;
        temp=get_Archive_List(hostChatter);
        ArchiveEntity[] result=new ArchiveEntity[temp.length];
        quickSort(temp,0,temp.length-1);
        
        for(int i=0;i<temp.length;i++)
        {
            result[i]=new ArchiveEntity();
            result[i]=temp[(temp.length-1)-i];
            
        }
        return result;
    }
    private ArchiveEntity[] get_Archive_List(String hostChatter)
    {

        File[] archFiles=null;
        File dir=new File(desPath + "\\" + hostChatter);
        archFiles=listFilesAsArray(dir);
        
        int count;
        count=0;
        String temp=new String();
        
        for(int i=0;i<archFiles.length;i++)
        {

            
            if (isArchiveFile(archFiles[i].getName()))
            {
                count++;
            }
        }
        
        
        ArchiveEntity[] result=new ArchiveEntity[count];
        for(int i=0;i<archFiles.length;i++)
        {
            if( !isArchiveFile(archFiles[i].getName()) )
                    continue;
            
            try
            {
                input=new Scanner(new File(archFiles[i].getPath()));
            }
            catch (FileNotFoundException fileNF)
            {
                System.out.println("Error: File not found");
            }
            String tmp=new String();
            tmp=archFiles[i].getName();
            tmp=tmp.split("-")[0];
            
            result[i]=new ArchiveEntity();
            
            result[i].guestChatter=tmp;
            tmp=archFiles[i].getName();
            tmp=tmp.split("-")[1];
            tmp=tmp.replace("_", "/");
            result[i].chatDate=tmp;
            tmp=archFiles[i].getName();
            tmp=tmp.split("-")[2];           
            tmp=tmp.replace("_", ":");
            tmp=tmp.replace(".archive", "");
            result[i].chatTime=tmp;
            result[i].hostChatter=hostChatter;
            result[i].text="";
            while(input.hasNextLine())
            {
                
                result[i].text=result[i].text + input.nextLine() + "\n";
            }

            
            
            
        }
        return result;
    }
    
    private boolean isArchiveFile(String fName)
    {
        String temp=new String();
        
        temp=fName.split("-")[2];
        temp=temp.substring(temp.length()-8);
        if(temp.equals(".archive"))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    public File[] listFilesAsArray(File directory)
    {
	return directory.listFiles();

    }

    
    public void quickSort (ArchiveEntity [] data, int low, int high)
    {
        int partitionIndex;
        if (high - low > 0)
        {
            partitionIndex = partition (data, low, high);
            quickSort (data, low, partitionIndex - 1);
            quickSort (data, partitionIndex + 1, high);
        }
    }

    private int partition (ArchiveEntity [] data, int low, int high)
    {
        int k, j;
        ArchiveEntity temp, p;
        p = data [low]; // Partition element
        // Find partition index (j).
        k = low;
        j = high + 1;
        do
        {
            k++;
        } while (data [k].date2Number() <= p.date2Number() && k < high);
        do
        {
            j--;
        } while (data [j].date2Number() > p.date2Number());

        while (k < j)
        {
            temp = data [k];
            data [k] = data [j];
            data [j] = temp;
            do
            {
                k++;
            } while (data [k].date2Number() <= p.date2Number());

            do
            {
                j--;
            } while (data [j].date2Number() > p.date2Number());
        }

        // Move partition element (p) to partition index (j).
        if (low != j)
        {
            temp = data [low];
            data [low] = data [j];
            data [j] = temp;
        }
        return j; // Partition index
    }
}
