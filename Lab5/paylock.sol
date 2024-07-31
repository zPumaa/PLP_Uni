pragma solidity >=0.4.16 <0.7.0;

contract Paylock {
    
    enum State { Working , Completed , Done_1 , Delay , Done_2 , Forfeit }
    
    int disc;
    State st;
    int clock;
    int private deadline1;
    address private timeAdd;
    
    constructor(address _timeAdd) public {
        st = State.Working;
        disc = 0;
        clock = 0;
        timeAdd = _timeAdd;
        
    }
    
    function getTick() public view returns (int) {
        return(clock);
    }
    
    function tick() external {
        require(msg.sender == timeAdd);
        clock += 1;
    }


    function signal() public {
        require( st == State.Working);
        st = State.Completed;
        disc = 10;
    }

    function collect_1_Y() public {
        require( st == State.Completed && clock < 4);
        st = State.Done_1;
        disc = 10;
    }

    function collect_1_N() external {
        require( st == State.Completed && clock >= 4);
        st = State.Delay;
        disc = 5;
        deadline1 = clock;
    }

    function collect_2_Y() external {
        require( st == State.Delay && clock < deadline1 + 4);
        st = State.Done_2;
        disc = 5;
    }

    function collect_2_N() external {
        require( st == State.Delay && clock >= deadline1 + 4);
        st = State.Forfeit;
        disc = 0;
    }

}

contract Supplier {
    
    Paylock p;
    Rental r;
    
    enum State { Working , Completed }
    
    State st;
    
    constructor(address pp, address rr) public {
        p = Paylock(pp);
        r = Rental(rr);
        st = State.Working;
    }
    
    function acquire_resource() public payable{
        require(st == State.Working);
        r.rent_out_resource.value(msg.value)();
    }
    
    function return_resource() public {
        require(st == State.Working);
        r.retrieve_resource();
    }
    
    function finish() external {
        require (st == State.Working);
        p.signal();
        st = State.Completed;
    }
    
}

contract Rental {
    
    address resource_owner;
    bool resource_available;
    uint constant deposit = 1 wei;
    
    constructor() public {
        resource_available = true;
    }
    
    function rent_out_resource() external payable {
        require(resource_available == true, "Resource not available");
        require(msg.value >= deposit, "Deposit amount not sent");
        resource_owner = msg.sender;
        resource_available = false;
    }

    function retrieve_resource() external {
        require(resource_available == false && msg.sender == resource_owner);
        (bool success, ) = msg.sender.call.value(deposit)("");
        require(success);
        resource_available = true;
    }
    
}
