using UnityEngine;
using System.Collections;

public class Shoot : MonoBehaviour {

    //Reference to the Audio File which is played when we fire our weapon
    public AudioClip FireAudioClip;

    //Reference to the GameObject which is used in order
    //to retrieve the Start position of our bullet-raycast (weapon's barrel end)
    public GameObject BulletStartPoint;

    //Reference to the ExplosionPrefab which is instantiated when an Enemy dies
    public GameObject ExplosionPrefab;

	// Use this for initialization
	void Start () {
        //Initialize the start position for our bullet-raycast
        BulletStartPoint = GameObject.FindGameObjectWithTag("BulletStartPoint");
	}
	
	// Update is called once per frame
	void Update () {

        //If the right mouse button is pressed...
        if(Input.GetMouseButtonDown(0))
        {
            Debug.Log("Hello");

            //Play the fire audio
            AudioSource.PlayClipAtPoint(FireAudioClip, transform.position);

            //Store the source for our raycast
            Vector3 StartLocation = BulletStartPoint.transform.position;

            //Hit will store useful information about which Object we hit with our raycast etc..
            RaycastHit Hit;

            //Start the actual raycast
            Physics.Raycast(StartLocation, BulletStartPoint.transform.forward, out Hit, 1000);

            //Display the previous raycast
            Debug.DrawRay(StartLocation, BulletStartPoint.transform.forward*1000, Color.blue, 5, false);

            //Store the GameObject we hit
            GameObject HitObject = Hit.transform.gameObject;

            //Log into the console the GameObject we hit
            Debug.Log(HitObject.ToString());

            //If the GameObject we hit is tagged as Enemy...
            if(HitObject.tag.Equals("Enemy"))
            {
                //Create an Explosion into our Explosion
                Instantiate(ExplosionPrefab, HitObject.transform.position, Quaternion.identity);

                //Destroy the enemy
                GameObject.Destroy(HitObject);

            }


        }
	
	}
}
