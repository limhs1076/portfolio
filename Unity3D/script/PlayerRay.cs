using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class PlayerRay : MonoBehaviour { 
    RaycastHit hit;
    public Ray ray;        // 다른 스크립트에서 레이 불러오기 위해 생성


    [SerializeField]
    private const float MAX_DISTANCE = 15f;
    [SerializeField]
    public IRaycastable raycastObj { get; private set; }
    [SerializeField]
    public GameObject pickUpObj { get; private set; }
    public Transform pickupLocation { get; private set; }

    public static bool isObjectPickUp { get; private set; }

    public Transform haingingPos = null;
    public PlayerHaingPosCheck playerHaingPosCheck = null;

    private DigitalRuby.LightningBolt.LightningBoltScript lightning;

    private Vector3 pickupObjScale;
    // Start is called before the first frame update
    void Start(){
        //pickupLocation = this.transform.parent.GetChild(1);
        //pickupLocation = this.gameObject.transform;
        pickupLocation = new GameObject().transform;
        //pickupLocation.SetParent(this.gameObject.transform.parent);
        pickupLocation.SetParent(this.gameObject.transform);
        try { 
        lightning = this.transform.Find("RemoteController").Find("LightningBolt").GetComponent<DigitalRuby.LightningBolt.LightningBoltScript>();
            lightning.gameObject.SetActive(false);
        }
        catch (NullReferenceException) {
            Debug.LogWarning("No Lightning!!!");
        }
        ray = new Ray();
        playerHaingPosCheck = haingingPos.GetChild(0).GetComponent<PlayerHaingPosCheck>();
    }
    void OnRayChanged() {
        try { 
            if(hit.transform.gameObject.GetComponent<IRaycastable>() != raycastObj) {
                if (raycastObj != null) {
                    raycastObj.OnRayOut();
                }
                raycastObj = hit.transform.GetComponent<IRaycastable>();
                raycastObj.OnRayHit();

                //Debug.Log("[Player Ray]: Ray Hits an Object: " + raycastObj.getGameObject().name);
            }
        }
        catch(NullReferenceException nullex) {
            if(raycastObj!= null) {
                raycastObj.OnRayOut();
                //Debug.Log(nullex.Message);
            }
        }
    }

    // Update is called once per frame
    void Update(){
        // ray 실시간 생성 & 변수화
        ray.origin = transform.position;
        ray.direction = transform.forward;
        if (Physics.Raycast(ray.origin, ray.direction, out hit, MAX_DISTANCE)) {

            if (hit.transform.gameObject.GetComponent<IRaycastable>() != null) {
                Debug.DrawRay(transform.position, transform.forward * MAX_DISTANCE, Color.red);
                OnRayChanged();
            } else {
                Debug.DrawRay(transform.position, transform.forward * MAX_DISTANCE, Color.blue);
                OnRayChanged();
            }

        } else {
            Debug.DrawRay(transform.position, transform.forward * MAX_DISTANCE, Color.black);
            OnRayChanged();
        }
        if (raycastObj != null)
            raycastObj.OnRayIng();
        //Debug.Log("raycastObj: "+ raycastObj+"\n" +
        //    "PlayerHaingPosCheck.hangingPossible: "+ playerHaingPosCheck.hangingPossible);
        if(raycastObj!= null && Input.GetButtonDown("Handle") && pickUpObj == null && playerHaingPosCheck.hangingPossible == true) {
            PickUpObj(raycastObj.getGameObject());
        } else if(pickUpObj != null && Input.GetButtonDown("Handle") && playerHaingPosCheck.hangingPossible == true) {
            PickDown();
        }else if(pickUpObj != null&& Input.GetButtonDown("Fire1") && playerHaingPosCheck.hangingPossible == true) {
            ShotObj();
        }
    }
    private Vector3 tempOriginVelo = Vector3.zero;
    public void PickUpObj(GameObject gameobject) {
        pickUpObj = gameobject;
        pickupObjScale = pickUpObj.transform.localScale;
        pickUpObj.transform.SetParent(pickupLocation.transform);
        //pickUpObj.transform.position = (this.transform.parent.forward * 1.5f) + this.transform.parent.position;
        pickUpObj.transform.position = haingingPos.transform.position;
        pickUpObj.transform.rotation = Quaternion.Euler(Vector3.zero);
        pickUpObj.transform.LookAt(this.transform.forward * 10f);
        pickUpObj.transform.rotation = Quaternion.Euler(Vector3.zero);
        tempOriginVelo = pickUpObj.GetComponent<Rigidbody>().velocity;
        pickUpObj.GetComponent<Rigidbody>().velocity = Vector3.zero;
        pickUpObj.GetComponent<Rigidbody>().useGravity = false;
        pickUpObj.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeAll;
        //pickUpObj.GetComponent<Rigidbody>().isKinematic = true;
        isObjectPickUp = true;


        try {
            lightning.gameObject.SetActive(true);
            lightning.EndObject = pickUpObj;
        }
        catch (NullReferenceException) {
            
        }
    }
    public void PickDown() {
        isObjectPickUp = false;
        //pickUpObj.GetComponent<Rigidbody>().isKinematic = false;
        //pickUpObj.GetComponent<Rigidbody>().useGravity = true;
        if(pickUpObj.GetComponent<ColorableObject>()?.icolor == pickUpObj.GetComponent<ColorableObject>()?.colorGravity) {
            pickUpObj.GetComponent<Rigidbody>().useGravity = false;
        } else {
            pickUpObj.GetComponent<Rigidbody>().useGravity = true;
        }
        pickUpObj.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
        //pickUpObj.GetComponent<Rigidbody>().velocity = tempOriginVelo;
        pickupLocation.DetachChildren();
        pickUpObj.transform.localScale = pickupObjScale;
        pickUpObj = null;



        try {
            lightning.gameObject.SetActive(false);
        }
        catch (NullReferenceException) {
            
        }
    }
    public void ShotObj() {
        isObjectPickUp = false;
        //pickUpObj.GetComponent<Rigidbody>().isKinematic = false;
        //pickUpObj.GetComponent<Rigidbody>().useGravity = true;
        if (pickUpObj.GetComponent<ColorableObject>()?.icolor == pickUpObj.GetComponent<ColorableObject>()?.colorGravity) {
            pickUpObj.GetComponent<Rigidbody>().useGravity = false;
        } else {
            pickUpObj.GetComponent<Rigidbody>().useGravity = true;
        }
        pickUpObj.GetComponent<Rigidbody>().velocity = Vector3.zero;
        pickUpObj.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
        pickupLocation.DetachChildren();
        pickUpObj.transform.localScale = pickupObjScale;
        //pickUpObj.GetComponent<Rigidbody>().AddForce(this.transform.parent.forward * 1000f);
        if (pickUpObj.GetComponent<ColorableObject>()?.icolor == pickUpObj.GetComponent<ColorableObject>()?.colorAcceleration) {
            pickUpObj.GetComponent<Rigidbody>().AddForce(this.transform.forward * 5000f);
        } else {
            pickUpObj.GetComponent<Rigidbody>().AddForce(this.transform.forward * 800f);
        }
        pickUpObj = null;

        try {
            lightning.gameObject.SetActive(false);
        }
        catch (NullReferenceException) {

        }
    }
    
}

