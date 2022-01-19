using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class PlayerInteraction : MonoBehaviour
{
    private static int ABILITY_NUM = 5;
    private RaycastHit hit;                 // �浹 ��ü
    private Material[] sphereMaterials = new Material[ABILITY_NUM + 1];      // 6�� ���͸����� SpOff -> 0�� ���׸����� off �� �ϵ��� 1104 ������
    private Renderer[] remoteSpRenderers = new Renderer[ABILITY_NUM];
    private Renderer mainSpRenderer;

    public Ability ability;                 // �÷��̾� ���� �ɷ�

    public bool abilityAllTrueCheck = true;
    public bool[] canUseAbility = new bool[ABILITY_NUM+1];            // �ɷ� ��� ���� ����

    List<int> usableAbil;
    int usablecnt;

    int nowAbilityIdx = 0;

    [ContextMenu("Reset Ability")]
    void resetAbility() {
        canUseAbility[0] = true;
        for (int i = 1; i < ABILITY_NUM + 1; i++)
            canUseAbility[i] = true;
    }

    void StartAbilitySet() {
        for(int i =1; i < ABILITY_NUM + 1; i++) {
            if (canUseAbility[i]) {
                remoteSpRenderers[i - 1].material = sphereMaterials[i];
            } else {
                remoteSpRenderers[i - 1].material = sphereMaterials[0];
            }
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        ability = Ability.nothing;
        //canUseAbility = new bool[ABILITY_NUM];
        //canUseAbility = new bool[ABILITY_NUM+1];
        if (abilityAllTrueCheck) {
            resetAbility();
        }
        canUseAbility[0] = true;

        //sphereMaterials[0] = Resources.Load<Material>("Sp1");
        //sphereMaterials[1] = Resources.Load<Material>("Sp2");
        //sphereMaterials[2] = Resources.Load<Material>("Sp3");
        //sphereMaterials[3] = Resources.Load<Material>("Sp4");
        //sphereMaterials[4] = Resources.Load<Material>("Sp5");
        //sphereMaterials[5] = Resources.Load<Material>("SpOff");
        sphereMaterials[0] = Resources.Load<Material>("SpOff");
        sphereMaterials[1] = Resources.Load<Material>("Sp1");
        sphereMaterials[2] = Resources.Load<Material>("Sp2");
        sphereMaterials[3] = Resources.Load<Material>("Sp3");
        sphereMaterials[4] = Resources.Load<Material>("Sp4");
        sphereMaterials[5] = Resources.Load<Material>("Sp5");

        mainSpRenderer = transform.Find("MainSp").GetComponent<Renderer>();
        remoteSpRenderers[0] = transform.Find("Sp1").GetComponent<Renderer>();
        remoteSpRenderers[1] = transform.Find("Sp2").GetComponent<Renderer>();
        remoteSpRenderers[2] = transform.Find("Sp3").GetComponent<Renderer>();
        remoteSpRenderers[3] = transform.Find("Sp4").GetComponent<Renderer>();
        remoteSpRenderers[4] = transform.Find("Sp5").GetComponent<Renderer>();
        StartAbilitySet();
    }


    void ColorChange() {
        for (int i = 0; i < ABILITY_NUM; i++) {
            if (canUseAbility[i] == false) {
                remoteSpRenderers[i].material = sphereMaterials[0];
            } else
                remoteSpRenderers[i].material = sphereMaterials[i];
        }
        //���� ���� ������ �ɷ����� �迭 ����
        usableAbil = new List<int>();
        usablecnt = 0;
        for (int i = 0; i < canUseAbility.Length; i++) {
            if (canUseAbility[i] == true) {
                usablecnt++;
                usableAbil.Add(i);
            }
        }
        //�ɷ� ����Ʈ��
        ability = (Ability)0;
        mainSpRenderer.material = sphereMaterials[0];

    }
    private bool keyCheck(int idx) {
        if(Input.GetKeyDown((KeyCode)48+idx) && canUseAbility[idx]) {
            return true;
        }
        return false;
    }

    private void AbilityChange(int idx) {
        ability = (Ability)idx;
        mainSpRenderer.material = sphereMaterials[idx];
        nowAbilityIdx = idx;
    }

    // Update is called once per frame
    void Update()
    {
        //// ��Ȱ��ȭ�� �ɷ��� RemoteSphere material �� ����
        //for (int i=0; i< ABILITY_NUM; i++)
        //{
        //    if (canUseAbility[i] == false)
        //    {
        //        remoteSpRenderers[i].material = sphereMaterials[0];
        //    }
        //    else
        //        remoteSpRenderers[i].material = sphereMaterials[i];
        //}

        if (!PlayerRay.isObjectPickUp) {
            #region regacycode
            // �ɷ� ����
            //if (Input.GetKeyDown(KeyCode.Alpha1) && canUseAbility[0])
            //{
            //    ability = Ability.magnetism;
            //    //Debug.Log("���� �ɷ�: Magnetism");
            //    mainSpRenderer.material = sphereMaterials[0];

            //}
            //else if (Input.GetKeyDown(KeyCode.Alpha2) && canUseAbility[1])
            //{
            //    ability = Ability.acceleration;
            //    //Debug.Log("���� �ɷ�: Acceleration");
            //    mainSpRenderer.material = sphereMaterials[1];
            //}
            //else if (Input.GetKeyDown(KeyCode.Alpha3) && canUseAbility[2])
            //{
            //    ability = Ability.gravity;
            //    //Debug.Log("���� �ɷ�: Gravity");
            //    mainSpRenderer.material = sphereMaterials[2];
            //}
            //else if (Input.GetKeyDown(KeyCode.Alpha4) && canUseAbility[3])
            //{
            //    ability = Ability.elasticity;
            //    //Debug.Log("���� �ɷ�: Elasticity");
            //    mainSpRenderer.material = sphereMaterials[3];
            //}
            //else if (Input.GetKeyDown(KeyCode.Alpha5) && canUseAbility[4])
            //{
            //    ability = Ability.invisible;
            //    //Debug.Log("���� �ɷ�: Invisible");
            //    mainSpRenderer.material = sphereMaterials[4];
            //}
            #endregion
            #region keyAbilitySelect
            if (keyCheck(0)) {
                AbilityChange(0);
            } else if (keyCheck(1)) {
                AbilityChange(1);
            }
            else if(keyCheck(2)) {
                AbilityChange(2);
            }
            else if(keyCheck(3)) {
                AbilityChange(3);
            }
            else if(keyCheck(4)) {
                AbilityChange(4);
            } else if (keyCheck(5)) {
                AbilityChange(5);
            }
            #endregion

            

            if(Input.mouseScrollDelta.y >= 1) {
                //���� �ɷ��� ���� �ɷ����� �̵�
                int findedAbil = -1;
                for(int i =nowAbilityIdx -1; i>= 0; i--) {
                    if(canUseAbility[i] == true) {
                        findedAbil = i;
                        break;
                    }
                }
                if(findedAbil == -1) {
                    for(int i = ABILITY_NUM; i>=nowAbilityIdx; i--) {
                        if (canUseAbility[i] == true) {
                            findedAbil = i;
                            break;
                        }
                    }
                }
                if(findedAbil == -1) {
                    findedAbil = 0;
                }
                AbilityChange(findedAbil);

            }else if(Input.mouseScrollDelta.y <= -1) {
                int findedAbil = -1;
                for (int i = nowAbilityIdx+1; i < ABILITY_NUM+1; i++) {
                    if (canUseAbility[i] == true) {
                        findedAbil = i;
                        break;
                    }
                }
                if (findedAbil == -1) {
                    for (int i = 0; i < nowAbilityIdx-1; i++) {
                        if (canUseAbility[i] == true) {
                            findedAbil = i;
                            break;
                        }
                    }
                }
                if (findedAbil == -1) {
                    findedAbil = 0;
                }

                AbilityChange(findedAbil);
            }
            // �Ӽ� �ο�/����
            //if (Input.GetMouseButtonDown(0))
            if (Input.GetButtonDown("Fire1")) {
                
                Physics.Raycast(transform.parent.position, transform.parent.forward, out hit);

               try
               {
                    GameObject hittedObject = hit.transform.gameObject;

                    // Colorable ť�� �����
                    if (hittedObject.GetComponent<IRaycastable>() != null && hittedObject.GetComponent<ColorableObject>() != null)
                    {
                        Icolor preveiousColorMode;  // ��ü�� �̹� ������ִ� ���º���
                        ColorableObject colorableObject = hittedObject.GetComponent<ColorableObject>();

                        preveiousColorMode = colorableObject.icolor;

                        // ���� ť���� �Ӽ��� ���ų� �Ӽ��� �ο��� ť�꿡 �ٸ� �Ӽ��� �ο��ϴ� ���
                        if (colorableObject.icolor.abilityState == Ability.nothing || ability != colorableObject.icolor.abilityState)
                        {
                            preveiousColorMode.ColorOff();
                            Debug.Log("�ɷ� �ο�");
                            switch (ability)
                            {
                                case Ability.magnetism:
                                    colorableObject.icolor = colorableObject.colorMagnetism;
                                    break;

                                case Ability.gravity:
                                    colorableObject.icolor = colorableObject.colorGravity;
                                    break;

                                case Ability.acceleration:
                                    colorableObject.icolor = colorableObject.colorAcceleration;
                                    break;

                                case Ability.invisible:
                                    colorableObject.icolor = colorableObject.colorInvisible;
                                    break;

                                case Ability.elasticity:
                                    colorableObject.icolor = colorableObject.colorElasticity;
                                    break;

                                default:
                                    colorableObject.icolor = colorableObject.colorNothing;
                                    break;
                            }
                            colorableObject.icolor.abilityState = ability;
                            colorableObject.icolor.ColorOn();
                        }
                        else
                        {
                            colorableObject.icolor.ColorOff();
                            colorableObject.icolor.abilityState = Ability.nothing;
                            Debug.Log("�ɷ� ����");

                        }
                    }
                }
                catch (NullReferenceException e)
                {
                    Debug.Log(e.ToString());
                }



            }
        }

        if (Input.GetKeyDown(KeyCode.Escape)) {
            Application.Quit();
        }
    }
}
