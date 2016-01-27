using UnityEngine;
using System.Collections;

public class HuntMeDown : MonoBehaviour {

    //GameObject which stores a reference to the player
    public GameObject Me;
	// Use this for initialization
	void Start () {
        //Searches the Object named FPSController (meaning the player in this case)
        Me = GameObject.Find("FPSController");
	}
	
	// Update is called once per frame
	void Update () {
        //Updates the destination of the NavMeshAgent component with the position of the player
        GetComponent<NavMeshAgent>().destination = Me.transform.position;
	}
}
