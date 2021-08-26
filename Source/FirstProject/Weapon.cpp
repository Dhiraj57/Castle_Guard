// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"


AWeapon::AWeapon() 
{
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(GetRootComponent());

    CombactCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
    CombactCollision->SetupAttachment(GetRootComponent());

    bWeaponParticle = false;

    WeaponState = EWeaponState::EWS_Pickup;

    Damage = 25.f;

}

void AWeapon::BeginPlay() 
{
    Super::BeginPlay();

    CombactCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombactOnOverlapBegin);
    CombactCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombactOnOverlapEnd);

    CombactCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombactCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombactCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombactCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if (Main)
        {
            Main->SetActiveOverlappingItem(this);
        }
        
    }
    
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

    if (OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if (Main)
        {
            Main->SetActiveOverlappingItem(nullptr);
        }
        
    }
}

void AWeapon::Equip(AMain* Char) 
{
    if (Char)
    {
        SetInstigator(Char->GetController());

        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

        SkeletalMesh->SetSimulatePhysics(false);

        const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
        if (RightHandSocket)
        {
            RightHandSocket->AttachActor(this, Char->GetMesh());
            bRotate = false;
            Char->SetEquippedWeapon(this);
            Char->SetActiveOverlappingItem(nullptr);
        }

        if (OnEquipSound)
        {
            UGameplayStatics::PlaySound2D(this, OnEquipSound);
        }

        if (!bWeaponParticle)
        {
            IdleParticlesComponent->Deactivate();
        }
        
        
    }
    
}

void AWeapon::CombactOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    if(OtherActor)
    {
        AEnemy* Enemy = Cast<AEnemy>(OtherActor);
        if(Enemy)
        {
            if(Enemy->HitParticles)
            {
                const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
                if(WeaponSocket)
                {
                    FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f), false);
                }
            }

            if(Enemy->HitSound)
            {
                UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
            }
            if(DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
            }
        }
    }

}

void AWeapon::CombactOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
    
}

void AWeapon::ActivateCollision() 
{
    CombactCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeActivateCollision() 
{
    CombactCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


