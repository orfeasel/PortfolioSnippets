using UnityEngine;
using System.Collections;

public class SpawnZombie : MonoBehaviour {

    //Zombie SpawnInterval
    public float SpawnInterval = 10;
    
    private float Timer;
    public GameObject EnemyPrefab;


	// Use this for initialization
	void Start () {
        //Initialize the timer
        Timer = SpawnInterval;
	}
	
	// Update is called once per frame
	void Update () {

        
        Timer -= Time.deltaTime;

        Debug.Log(Timer.ToString());

        //If enough time has passed, spawn a new zombie
        if(Timer<=0)
        {
            Instantiate(EnemyPrefab, transform.position, Quaternion.identity);
            Timer = SpawnInterval;
        }



	}
}
